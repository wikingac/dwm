/* See LICENSE file for copyright and license details. */

/*Hack  appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 20;       /* gaps between windows */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack Nerd Font:size=13.5", "NotoColorEmoji:pixelsize=15.5:antialias=true:autohint=true" };
static const char dmenufont[]       =   "monospace:size=13.4";
static const char col_gray1[]       = "#222222";
static const char col_border[]      = "#f5b60a";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#285577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray1 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class          instance    title       tags mask     isfloating   monitor */
	{ "Gimp",	  NULL,       NULL,       0,            1,           -1 },
	{ "zathura",	  NULL,       NULL,       0,            1,           -1 },
	{ "vlc",	  NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     	= 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     	= 1;    /* number of clients in master area */
static const int resizehints 	= 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; 	/* 1 will force focus on the fullscreen window */
static const int attachbelow 	= 1;    /* 1 means attach after the currently active window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "|M|",      centeredmaster },
	{ "HHH",      grid },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "[M]",      monocle },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* terminal emulator */
#define TERMINAL "st"

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }


/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERMINAL, "-e", "zsh", NULL };
static const char *diskmg[]   = { TERMINAL, "-e", "ncdu", NULL };
static const char *filemg[]   = { TERMINAL, "-e", "lf", NULL };
static const char *taskmg[]   = { TERMINAL, "-e", "btop", NULL };
static const char *soundctl[] = { TERMINAL, "-e", "ncpamixer", "-t", "o", NULL };
static const char *musicply[] = { TERMINAL, "-e", "ncmpcpp", NULL };
static const char *vimwiki[]  = { TERMINAL, "-e", "nvim", "-c", "VimwikiIndex", NULL };
static const char *rss[]      = { TERMINAL, "-e", "/bin/sh", "-c", "source ~/.local/bin/proxy && newsboat", NULL };
static const char *clock[]    = { TERMINAL, "-e", "tty-clock", "-c", "-s", "-C", "3", NULL };
static const char *volup[]    = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%",    NULL };
static const char *voldown[]  = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%",    NULL };
static const char *volreset[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "30%",    NULL };
static const char *volmute[]  = { "pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle", NULL };
static const char *playerctl[]= { "mpc", "toggle", NULL };
static const char *playnext[] = { "mpc", "next", NULL };
static const char *playprev[] = { "mpc", "prev", NULL };
static const char *locksc[]   = { "/bin/sh", "-c", "slock & mpc pause & ~/.local/bin/pausempv", NULL };
static const char *setbg[]    = { "/bin/sh", "-c", "~/.local/bin/setbg", NULL };
static const char *mpvctl[]   = { "/bin/sh", "-c", "~/.local/bin/mpvtoggle", NULL };
static const char *ss[]	      = { "flameshot", "gui",  NULL };
static const char *ssfull[]   = { "flameshot", "full", NULL };
static const char *browser[]  = { "firefox", NULL };
static const char *brow_p[]   = { "librewolf", NULL };
static const char *chrom[]    = { "chromium", "--proxy-server=127.0.0.1:7897", NULL };
static const char *dict[]     = { "goldendict", NULL };
static const char *bookmg[]   = { "calibre", NULL };
static const char *clash[]    = { "clash-verge", NULL };

static const Key keys[] = {
	/* modifier                     key                       function        argument */
	{ MODKEY|ShiftMask,             XK_Return,		  spawn,          {.v = termcmd  } },
	{ MODKEY|ShiftMask,             XK_d,			  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_a,		          spawn,          {.v = ss       } },
	{ MODKEY|ShiftMask,             XK_b,			  spawn,          {.v = browser  } },
	{ MODKEY|ShiftMask,             XK_u,			  spawn,          {.v = chrom    } },
	{ MODKEY|ShiftMask,             XK_o,			  spawn,          {.v = brow_p   } },
	{ MODKEY|ShiftMask,             XK_r,			  spawn,          {.v = filemg   } },
	{ MODKEY|ShiftMask,             XK_n,			  spawn,          {.v = soundctl } },
	{ MODKEY|ShiftMask,             XK_m,			  spawn,          {.v = musicply } },
	{ MODKEY|ShiftMask,             XK_k,			  spawn,          {.v = rss      } },
	{ MODKEY|ShiftMask,             XK_j,			  spawn,          {.v = vimwiki  } },
	{ MODKEY|ShiftMask,             XK_l,			  spawn,          {.v = locksc   } },
	{ MODKEY|ShiftMask,             XK_w,			  spawn,          {.v = dict     } },
	{ MODKEY|ShiftMask,             XK_s,    		  spawn,          {.v = taskmg   } },
	{ MODKEY|ShiftMask,             XK_v,			  spawn,          {.v = bookmg   } },
	{ MODKEY|ShiftMask,             XK_x,			  spawn,          {.v = clash    } },
	{ MODKEY|ShiftMask,             XK_g,			  spawn,          {.v = setbg    } },
	{ MODKEY|ShiftMask,             XK_y,    		  spawn,          {.v = diskmg   } },
	{ MODKEY|ShiftMask,             XK_e,   		  spawn,          {.v = clock    } },
	{ MODKEY|ShiftMask,		XK_Print,		  spawn,          {.v = ssfull   } },
	{ MODKEY|ShiftMask,             XK_Left,                  spawn,          {.v = playprev } },
	{ MODKEY|ShiftMask,             XK_Right,                 spawn,          {.v = playnext } },
	{ MODKEY,                       XK_p,                     spawn,          {.v = playerctl} },
	{ MODKEY|ShiftMask,             XK_p,                     spawn,          {.v = mpvctl   } },
	{ 0,				XF86XK_AudioLowerVolume,  spawn,	  {.v = voldown  } },
	{ 0,				XF86XK_AudioRaiseVolume,  spawn,	  {.v = volup    } },
	{ 0,				XF86XK_AudioMute,	  spawn,	  {.v = volmute  } },
	{ ControlMask,                  XF86XK_AudioRaiseVolume,  spawn,          {.v = volreset } },
	{ MODKEY,                       XK_b,			  togglebar,      {0} },
	{ MODKEY,                       XK_j,			  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,			  focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,			  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,			  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,			  setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,			  setmfact,       {.f = +0.05} },
        { MODKEY|ShiftMask,             XK_h,                     setcfact,       {.f = +0.25} },
	{ MODKEY,                       XK_Return,		  zoom,           {0} },
	{ MODKEY,                       XK_Tab,			  view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,			  killclient,     {0} },
	{ MODKEY,                       XK_t,			  setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,			  setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_g,			  setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,			  setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,		  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,		  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,			  togglefullscr,  {0} },
	{ MODKEY,                       XK_0,			  view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,			  tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,		  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,		  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,		  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,		  tagmon,         {.i = +1 } },
	/* not in use but to remove warnings*/
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,			  incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,			  incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,			  incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,			  incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,			  incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,			  incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,			  incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_0,			  togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,			  defaultgaps,    {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_Delete,                quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

