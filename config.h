/* See LICENSE file for copyright and license details. */

/* constants */
#define TERMINAL "kitty"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",		tile },    		/* Default: Master on left, slaves on right */
	{ "TTT",		bstack },		/* Master on top, slaves on bottom */

	{ "[M]",		monocle },

	{ "><>",      NULL },    			/* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, 	XK_k,   ACTION##stack, {.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack, {.i = 0 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/* OBSOLETE???? */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };

/* #include <X11/XF86keysym.h> */

static Key keys[] = {
	/* modifier                     key        	function        argument */
	STACKKEYS(MODKEY,				focus)
	STACKKEYS(MODKEY|ShiftMask,			push)
	/* { MODKEY,			XK_Escape,	spawn, 		SHCMD("") } */
	/* { MODKEY|ShiftMask,		XK_Escape,	spawn,		SHCMD("") }, */

	/* { MODKEY,			XK_onehalf,	spawn,		SHCMD("") } */
	/* { MODKEY|ShiftMask,		XK_onehalf,	spawn,		SHCMD("") } */
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)
	{ MODKEY,			XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,			XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 5") },
	{ MODKEY|ShiftMask,		XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 15") },
	{ MODKEY,			XK_plus,	spawn,		SHCMD("pamixer --allow-boost -i 5") },
	{ MODKEY|ShiftMask,		XK_plus,	spawn,		SHCMD("pamixer --allow-boost -i 15") },
	/* { MODKEY,			XK_dead_acute,	spawn,		SHCMD(""), */
	/* { MODKEY|ShiftMask,		XK_dead_acute,	spawn,		SHCMD(""), */
	/* { MODKEY,			XK_BackSpace,	spawn,		SHCMD("") }, */
	/* { MODKEY|ShiftMask,		XK_BackSpace,	spawn,		SHCMD("") }, */

	{ MODKEY,			XK_Tab,		view,		{0} },
	/* { MODKEY|ShiftMask,		XK_Tab,		spawn,		SHCMD("") }, */
	{ MODKEY,			XK_q,		killclient,	{0} },
	{ MODKEY|ShiftMask,		XK_q,		spawn,		SHCMD("pwrmenu") },
	{ MODKEY,			XK_w,		spawn,		SHCMD("$BROWSER") },
	/* { MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD(TERMINAL " -e sudo nmtui") }, */
	/* { MODKEY,			XK_e,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") }, */
	/* { MODKEY|ShiftMask,		XK_e,		spawn,		SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") }, */
	{ MODKEY,			XK_r,		spawn,		SHCMD(TERMINAL " -e lf") },
	/* { MODKEY|ShiftMask,		XK_r,		spawn,		SHCMD(TERMINAL " -e htop") }, */
	{ MODKEY,			XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	/* { MODKEY,			XK_y,		setlayout,	{.v = &layouts[2]} }, */ /* spiral */
	/* { MODKEY|ShiftMask,		XK_y,		setlayout,	{.v = &layouts[3]} }, */ /* dwindle */
	/* { MODKEY,			XK_u,		setlayout,	{.v = &layouts[4]} }, */ /* deck */
	/* { MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[5]} }, */ /* monocle */
	/* { MODKEY,			XK_i,		setlayout,	{.v = &layouts[6]} }, */ /* centeredmaster */
	/* { MODKEY|ShiftMask,		XK_i,		setlayout,	{.v = &layouts[7]} }, */ /* centeredfloatingmaster */
	{ MODKEY,			XK_o,		incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_o,		incnmaster,     {.i = -1 } },
	{ MODKEY,			XK_p,		spawn,		SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,		XK_p,		spawn,		SHCMD("mpc pause ; pauseallmpv") },
	{ MODKEY,			XK_ae,		spawn,		SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,		XK_ae,		spawn,		SHCMD("mpc seek -60") },
	{ MODKEY,			XK_oslash,	spawn,		SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,		XK_oslash,	spawn,		SHCMD("mpc seek +60") },
	/* { MODKEY,			XK_aring,	spawn,		SHCMD("") }, */
	/* { MODKEY|ShiftMask,		XK_aring,	spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_dead_diaresis, spawn,	SHCMD("") }, */
	/* { MODKEY,ShiftMask,		XK_dead_diaresis, spawn,	SHCMD("") }, */

	/* { MODKEY,			XK_a,		togglegaps,	{0} }, */
	/* { MODKEY|ShiftMask,		XK_a,		defaultgaps,	{0} }, */
	/* { MODKEY,			XK_s,		togglesticky,	{0} }, */
	/* { MODKEY|ShiftMask,		XK_s,		spawn,		SHCMD("") }, */
	{ MODKEY,			XK_d,		spawn,          SHCMD("rofi -show drun") },
	/* { MODKEY|ShiftMask,		XK_d,		spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_f,		togglefullscr,	{0} }, */
	/* { MODKEY|ShiftMask,		XK_f,		setlayout,	{.v = &layouts[8]} }, */
	/* { MODKEY,			XK_g,		shiftview,	{ .i = -1 } }, */
	/* { MODKEY|ShiftMask,		XK_g,		shifttag,	{ .i = -1 } }, */
	{ MODKEY,			XK_h,		focusmon,	{.i = -1} },
	{ MODKEY|ShiftMask,		XK_h,		tagmon,		{.i = -1} },
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY,			XK_l,		focusmon,      	{.i = +1} },
	{ MODKEY|ShiftMask,		XK_l,		tagmon,      	{.i = +1} },
	/* { MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 0} }, */
	/* { MODKEY|ShiftMask,		XK_apostrophe,	togglescratch,	{.ui = 1} }, */
	{ MODKEY,			XK_Return,	spawn,		{.v = termcmd } },
	/* { MODKEY|ShiftMask,		XK_Return,	spawn,		SHCMD("") }, */

	/* { MODKEY,			XK_less,	spawn,		SHCMD("") }, */
	/* { MODKEY|ShiftMask,		XK_less,	spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_z,		incrgaps,	{.i = +5 } }, */
	/* { MODKEY|ShiftMask,		XK_z,		spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_x,		incrgaps,	{.i = -5 } }, */
	/* { MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_c,		spawn,		SHCMD("") }, */
	/* { MODKEY|ShiftMask,		XK_c,		spawn,		SHCMD("") }, */
	/* V is automatically bound above in STACKKEYS */
	{ MODKEY,			XK_b,		togglebar,	{0} },
	/* { MODKEY|ShiftMask,		XK_b,		spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_n,		spawn,		SHCMD(TERMINAL " -e nvim -c VimwikiIndex") }, */
	{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD(TERMINAL " -e newsboat") },
	{ MODKEY,			XK_m,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ MODKEY|ShiftMask,		XK_m,		spawn,		SHCMD("pamixer -t") },
	{ MODKEY,			XK_comma,	spawn,		SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,		XK_comma,	spawn,		SHCMD("mpc seek 0%") },
	{ MODKEY,			XK_period,	spawn,		SHCMD("mpc next") },
	{ MODKEY|ShiftMask,		XK_period,	spawn,		SHCMD("mpc repeat") },

	/* { MODKEY,			XK_F1,		spawn,		SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") }, */
	/* { MODKEY,			XK_F2,		spawn,		SHCMD("tutorialvids") }, */
	/* { MODKEY,			XK_F3,		spawn,		SHCMD("displayselect") }, */
	/* { MODKEY,			XK_F4,		spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") }, */
	/* { MODKEY,			XK_F5,		xrdb,		{.v = NULL } }, */
	/* { MODKEY,			XK_F6,		spawn,		SHCMD("torwrap") }, */
	/* { MODKEY,			XK_F7,		spawn,		SHCMD("td-toggle") }, */
	/* { MODKEY,			XK_F8,		spawn,		SHCMD("mw -Y") }, */
	/* { MODKEY,			XK_F9,		spawn,		SHCMD("dmenumount") }, */
	/* { MODKEY,			XK_F10,		spawn,		SHCMD("dmenuumount") }, */
	/* { MODKEY,			XK_F11,		spawn,		SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") }, */
	/* { MODKEY,			XK_F12,		spawn,		SHCMD("remaps & notify-send \\\"⌨️ Keyboard remapping...\\\" \\\"Re-running keyboard defaults for any newly plugged-in keyboards.\\\"") }, */
	{ MODKEY,			XK_space,	zoom,		{0} },
	{ MODKEY|ShiftMask,		XK_space,	togglefloating,	{0} },

	{ MODKEY,			XK_Left,	setmfact,	{.f = -0.05}, },
	/* { MODKEY|ShiftMask,		XK_Left,	,		, }, */
	{ MODKEY,			XK_Right,	setmfact,	{.f = +0.05}, },
	/* { MODKEY|ShiftMask,		XK_Right,	,		, }, */
	/* { MODKEY,			XK_Up,		,		, }, */
	/* { MODKEY|ShiftMask,		XK_Up,		,		, }, */
	/* { MODKEY,			XK_Down,	,		, }, */
	/* { MODKEY|ShiftMask,		XK_Down,	,		, }, */

	{ 0,				XK_Print,	spawn,		SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,			XK_Print,	spawn,		SHCMD("maimpick") },
	/* { MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord") }, */
	/* { MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("dmenurecord kill") }, */
	/* { MODKEY,			XK_Delete,	spawn,		SHCMD("dmenurecord kill") }, */
	/* { MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") }, */
	/*
	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer --allow-boost -i 5") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer --allow-boost -d 5") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	*/
	/* { 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") }, */
	/* { 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") }, */
	/* { 0, XF86XK_AudioMedia,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") }, */
	/*
	{ 0, XF86XK_AudioMicMute,	spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	*/
	/* { 0, XF86XK_PowerOff,		spawn,		SHCMD("pwrmenu") }, */
	/* { 0, XF86XK_Calculator,		spawn,		SHCMD(TERMINAL " -e bc -l") }, */
	/* { 0, XF86XK_Sleep,		spawn,		SHCMD("sudo -A zzz") },*/
	/* { 0, XF86XK_WWW,		spawn,		SHCMD("$BROWSER") }, */
	/* { 0, XF86XK_DOS,		spawn,		SHCMD(TERMINAL) }, */
	/* { 0, XF86XK_ScreenSaver,	spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") }, */
	/* { 0, XF86XK_TaskPane,		spawn,		SHCMD(TERMINAL " -e htop") }, */
	/* { 0, XF86XK_Mail,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") }, */
	/* { 0, XF86XK_MyComputer,	spawn,		SHCMD(TERMINAL " -e lf /") }, */
	/* { 0, XF86XK_Battery,		spawn,		SHCMD("") }, */
	/* { 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") }, */
	/* { 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") }, */
	/* { 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") }, */
	/* { 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") }, */
	/*
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("xbacklight -inc 15") },
	*/
	/* { 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") },	*/
};
/* COMMENT */
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

