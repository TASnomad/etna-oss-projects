import json
from brain.Brain import Brain

# TODO: Delete when plugged with Coco's part
#notification = '{"notification_type": 0, "data": {"game_status": 1, "players": [{"y": 1, "x": 4, "energy": 40, "looking": 1, "id": "#0x08"}], "map_size": 5, "energy_cells": [{"y": 4, "x": 2, "value": 10}, {"y": 4, "x": 4, "value": 13}, {"y": 4, "x": 1, "value": 9}, {"y": 3, "x": 4, "value": 10}, {"y": 1, "x": 4, "value": 5}]}}'

class AI:
    def __init__(self, id):
        self.id = id
        self.brain = Brain()

    # Create a map from notification's data
    # 1: Empty field (can't be 0 because of the library I use for pathfinding)
    # 2: Player location
    # 3: Energy location
    def createMap(self, energy_cells, map_size, players):
        map = [[1 for _ in range(map_size)] for _ in range(map_size)]
        for i in range(len(energy_cells) - 1):
            map[energy_cells[i]['y']][energy_cells[i]['x']] = 3
        for i in range(len(players) - 1):
            if players[i]['id'] == self.id:
                self.brain.setPlayerData(players[i])
                map[players[i]['y']][players[i]['x']] = 2
        # Uncomment to see the map created
        # print('\n'.join(['\t'.join([str(cell) for cell in row]) for row in map]))
        return map

    def chooseActions(self, notification):
        data = json.loads(notification)['data']
        map = self.createMap(data['energy_cells'], data['map_size'], data['players'])
        self.brain.setMap(map)
        return self.brain.process()


#ai = AI('#0x08')
#ai.chooseActions(notification)
