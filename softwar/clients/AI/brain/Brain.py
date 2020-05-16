# import sys
# sys.path.append('..')

from ..pathfinding.core.grid import Grid
from ..pathfinding.finder.a_star import AStarFinder
from ..pathfinding.core.diagonal_movement import DiagonalMovement

class Brain:
    def __init__(self):
        self.map = []
        self.actions = []
        self.playerData = {}
        self.looksModifiers = {
            0: { 'x': -1, 'y': 0 }, # 'left'
            1: { 'x': 0, 'y': -1 }, # 'up'
            2: { 'x': 1, 'y': 0 },  # 'right'
            3: { 'x': 0, 'y': 1 },  # 'down'
        }

    def setMap(self, map):
        self.map = map

    def setPlayerData(self, playerData):
        self.playerData = playerData

    def getTargetCoord(self):
        for y in range(len(self.map)):
            for x in range(len(self.map[y])):
                if self.map[y][x] == 3:
                    return x, y
        return 'Ooops, looks like there\'s not any energy left in the map!'

    def getPath(self, playerCoord):
        grid = Grid(matrix=self.map)
        start = grid.node(playerCoord[0], playerCoord[1])
        end = grid.node(self.targetCoord[0], self.targetCoord[1])
        finder = AStarFinder(diagonal_movement=DiagonalMovement.never)
        return finder.find_path(start, end, grid)

    def getLookingCoord(self, playerCoord):
        x = playerCoord[0] + self.looksModifiers[self.playerData['looking']]['x']
        y = playerCoord[1] + self.looksModifiers[self.playerData['looking']]['y']
        return x, y

    def orientate(self, start, end):
        x = end[0] - start[0]
        y = end[1] - start[1]
        mult = 1
        if (self.playerData['looking'] == 0 or self.playerData['looking'] == 3):
            mult = -1
        if x * mult == 1:
            self.actions.append('right')
        elif x * mult == -1:
            self.actions.append('left')
        elif abs(x) == 2 or abs(y) == 2:
            self.actions.extend(('left', 'left'))
        else:
            return 'Ooops, looks like an something went wrong!'

    def process(self):
        playerCoord = (self.playerData['x'], self.playerData['y'])
        self.targetCoord = self.getTargetCoord()
        path = self.getPath(playerCoord)
        lookingCoord = self.getLookingCoord(playerCoord)
        self.orientate(lookingCoord, path[0][1])
        if (len(self.actions) == 2):
            return self.actions
        self.actions.append('forward')
        return self.actions            
