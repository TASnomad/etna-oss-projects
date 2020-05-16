#include	<stdio.h>
#include	<SDL2/SDL.h>
#include 	<execinfo.h>
#include	<signal.h>

#include	<proto.h>
#include	<map.h>
#include	<window.h>
#include	<gameEvent.h>
// #undef		main

void handler(int sig) {
  void *array[128];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 128);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}



#define		__unused __attribute__((unused))

int		main(__unused int argc, __unused char **argv)
{
	// struct sigaction sa;
	// sigset_t	st;

	// sigemptyset(&st);
	// sigaddset(&st, SIGPIPE);
	// sa.sa_mask = st;
	// sa.sa_handler = 0;
	// sa.sa_flags = 0;
	// sigaction(SIGPIPE, &sa, NULL);

	signal(SIGSEGV, handler);
	t_window * window = create_bomberman_window();
	SDL_Event e;
	window->toDisplay = 1;

	handle_display(window);
	SDL_StartTextInput();
	if (!window)
		return (-1);

	while (window != NULL && window->still_running == 1)
	{
		if (SDL_PollEvent(&e))
			handleEvent(window, e);
	}
	free_t_window(window);

	SDL_StopTextInput();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return (0);
}
