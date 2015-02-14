#include <stdio.h>
#include <xcb/xcb.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

#define BORDERWIDTH 4

static xcb_connection_t *conn;
static xcb_screen_t *scr;

static void cleanup (void);
static void killwin (xcb_window_t);
static void center_pointer (xcb_window_t);
static void nextwin (xcb_window_t);
static void resize (xcb_window_t, int, int);
static void move (xcb_window_t, int, int);

static void
cleanup (void) {
	if (conn)
		xcb_disconnect(conn);
}

static void
killwin (xcb_window_t win) {
	if (!win || win == scr->root)
		return;

	xcb_kill_client(conn, win);
	nextwin(win);
}

static void
center_pointer (xcb_window_t win) {
	uint32_t values[1];
	xcb_get_geometry_reply_t *geom;
	geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);

	if (!geom)
		errx(1, "center_pointer: missing geometry!");

	xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0,
			(geom->width  + (BORDERWIDTH * 2)) / 2,
			(geom->height + (BORDERWIDTH * 2)) / 2);

	values[0] = XCB_STACK_MODE_ABOVE;
	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, values);
}

static void
nextwin (xcb_window_t win) {
	xcb_query_tree_reply_t *r;
	xcb_window_t *c,t = 0;
	xcb_get_window_attributes_cookie_t ac;
	xcb_get_window_attributes_reply_t *ar;

	r = xcb_query_tree_reply(conn, xcb_query_tree(conn, scr->root), 0);
	if (!r || !r->children_len)
		return;
	c = xcb_query_tree_children(r);
	for (unsigned int i=0; i < r->children_len; i++) {
		ac = xcb_get_window_attributes(conn, c[i]);
		ar = xcb_get_window_attributes_reply(conn, ac, NULL);

		if (ar && ar->map_state == XCB_MAP_STATE_VIEWABLE) {
			if (!(ar->override_redirect || c[i] == win)) {
				t = c[i];
			}
			break;
		}
	}

	if (t)
		center_pointer(t);

	free(r);
}

static void
resize (xcb_window_t win, int x, int y) {
	uint32_t values[2];
	int real;
	xcb_get_geometry_reply_t *geom;

	if (!win || win == scr->root)
		return;
	geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);

	if (!geom)
		return;

	values[0] = x ? geom->width + x : geom->width;
	values[1] = y ? geom->height + y : geom->height;

	if (x)
	{
		real = geom->width + (BORDERWIDTH * 2);
		if (geom->x + real + x > scr->width_in_pixels)
			values[0] = scr->width_in_pixels - geom->x -
				(BORDERWIDTH * 2);
	}

	if (y)
	{
		real = geom->height + (BORDERWIDTH *2);
		if (geom->y + real + y > scr->height_in_pixels)
			values[1] = scr->height_in_pixels - geom->y -
				(BORDERWIDTH * 2);
	}

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_WIDTH
			| XCB_CONFIG_WINDOW_HEIGHT, values);

	center_pointer(win);

	free(geom);
}

static void
move (xcb_window_t win, int x, int y) {
	uint32_t values[2];
	int real;
	xcb_get_geometry_reply_t *geom;

	if (!win || win == scr->root)
		return;

	geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);
	if (!geom)
		return;

	values[0] = x ? geom->x + x : geom->x;
	values[1] = y ? geom->y + y : geom->y;

	if (x)
	{
		real = geom->width + (BORDERWIDTH * 2);
		if (geom->x + x < 1)
			values[0] = 0;
		if (geom->x + x > scr->width_in_pixels - real)
			values[0] = scr->width_in_pixels - real;
	}

	if (y)
	{
		real = geom->height + (BORDERWIDTH * 2);
		if (geom->y + y < 1)
			values[1] = 0;
		if (geom->y + y > scr->height_in_pixels - real)
			values[1] = scr->height_in_pixels - real;
	}

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_X
			| XCB_CONFIG_WINDOW_Y, values);


	center_pointer(win);
	free(geom);
}

int main (int argc, char **argv) {
	atexit(cleanup);
	xcb_window_t *focuswin;
	xcb_window_t win;
	if (xcb_connection_has_error(conn = xcb_connect(NULL, NULL)))
		errx(1, "error connecting to xcb");

	scr = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

	key_t key = 0x0DEADCA7;
	int shmid;

	if ((shmid = shmget(key, sizeof(xcb_window_t), 0666)) < 0)
		warnx("shmget error");

	if ((focuswin = shmat(shmid, NULL, 0)) == (xcb_window_t *) -1)
		warnx("shmat error");

	win  = *focuswin;
	if (!win)
		errx(1, "no window");

	if (argc >= 2 && argv[1][0])
		switch (argv[1][1])
		{
			case 'm':
				if (argc != 4)
					return 1;
				
				switch (argv[2][0])
				{
					case 'x':
					case 'X':
						move(win, atoi(argv[3]), 0);
						break;
					case 'y':
					case 'Y':
						move(win, 0, atoi(argv[3]));
						break;
				}
			break;

			case 'r':
				if (argc != 4)
					return 1;

				switch (argv[2][0])
				{
					case 'x':
					case 'X':
						resize(win, atoi(argv[3]), 0);
						break;
					case 'y':
					case 'Y':
						resize(win, 0, atoi(argv[3]));
						break;
				}
			break;

			case 'f':
				if (argc != 3)
					return 1;

				switch (argv[2][0])
				{
					case 'n':
					case 'N':
						nextwin(win);
						break;
				}
			break;

			case 'k':
				killwin(win);
			break;
		}

	xcb_flush(conn);

	return 0;
}
