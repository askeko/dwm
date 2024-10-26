/* See LICENSE file for copyright and license details. */

/* CONSTANTS */
#include <X11/X.h>
#define TERMINAL "kitty"

/* APPEARANCE */
static const unsigned int borderpx = 3; /* border pixel of windows */
static const unsigned int snap = 32;    /* Snap pixel */
static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to
          monitorX */
static const unsigned int systrayonleft =
    0; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 5; /* systray spacing */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor */
static const int showsystray = 1; /* 0 means no systray */
static const int swallowfloating =
    0; /* 1 means swallow floating windows by default */
static const unsigned int gappih = 20; /* inner horizontal gaps */
static const unsigned int gappiv = 10; /* inner vertical gaps */
static const unsigned int gappoh = 10; /* outer horizontal gaps */
static const unsigned int gappov = 30; /* outer vertical gaps */
static int smartgaps =
    1; /* 1 means no outer gap when there is only one window */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
static const int use_sigusr1_dwmblocks =
    1; /* 0: use real time signals, 1: use sigusr1 (dwmblocks) */

/* Display modes of the tab bar: never shown, always shown, shown only in   */
/* monocle mode in presence of several windows.                             */
/* Modes after showtab_nmodes are disabled                                  */
enum showtab_modes {
  showtab_never,
  showtab_auto,
  showtab_nmodes,
  showtab_always
};

static const int showtab = showtab_auto; /* Default tab bar show mode */
static const Bool toptab = False;        /* False means bottom tab bar */
static const unsigned int colorfultag =
    1;                            /* 0 means use SchemeSel for selected tag */
static const int horizpadbar = 5; /* horizontal padding for statusbar */
static const int vertpadbar = 12; /* vertical padding for statusbar */

static const char *fonts[] = {
    "FiraCode Nerd Font Mono:style:medium:size=12",
};

static const char dmenufont[] = "monospace:size=10";

/* THEME */
#include "themes/tokyo_night.h"

static const char *colors[][3] = {
    /*                 fg       bg      border */
    [SchemeNorm] = {fg, bg, gray2},   [SchemeSel] = {gray4, blue, blue},
    [SchemeTitle] = {fg, bg, bg},     [SchemeTag] = {fg, bg, bg},
    [SchemeTag1] = {blue, bg, bg},    [SchemeTag2] = {red, bg, bg},
    [SchemeTag3] = {green, bg, bg},   [SchemeTag4] = {yellow, bg, bg},
    [SchemeTag5] = {purple, bg, bg},  [SchemeTag6] = {teal, bg, bg},
    [SchemeTag7] = {orange, bg, bg},  [SchemeTag8] = {cyan, bg, bg},
    [SchemeTag9] = {magenta, bg, bg}, [SchemeLayout] = {blue, gray2, bg},
};

/* SCRATCHPADS */
typedef struct {
  const char *name;
  const void *cmd;
} Sp;

// TODO: needs fixing! See https://github.com/wez/wezterm/discussions/2311 ...
// use initial_cols, initial_rows?
const char *spcmd1[] = {TERMINAL,
                        "--name",
                        "spterm",
                        "-o",
                        "remember_window_size=no",
                        "-o",
                        "initial_window_width=500",
                        "-o",
                        "initial_window_height=200",
                        NULL};
const char *spcmd2[] = {TERMINAL,
                        "--name",
                        "spcalc",
                        "-o",
                        "font_size=16",
                        "-o",
                        "remember_window_size=no",
                        "-o",
                        "initial_window_width=480",
                        "-o",
                        "initial_window_height=408",
                        "bc",
                        "-lq",
                        NULL};
static Sp scratchpads[] = {
    /* name     cmd */
    {"spterm", spcmd1},
    {"spcalc", spcmd2},
};

/* TAGGING */
static const char *tags[] = {"󰣇", "",  "",  "󰈹", "",
                             "󰓇", "󰣇", "󰣇", "󰣇"};

/* default layout per tags */
/* The first element is for all-tag view, following i-th element corresponds to
 */
/* tags[i]. Layout is referred using the layouts array index. */
static int def_layouts[1 + LENGTH(tags)] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int tagschemes[] = {SchemeTag1, SchemeTag2, SchemeTag3,
                                 SchemeTag4, SchemeTag5, SchemeTag6,
                                 SchemeTag7, SchemeTag8, SchemeTag9};

static const unsigned int ulinepad =
    5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke =
    2; /* thickness / height of the underline */
static const unsigned int ulinevoffset =
    0; /* how far above the bottom of the bar the line should appear */
static const int ulineall =
    0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     * WM_CLASS(STRING) = instance, class
     * WM_NAME(STRING) = title
     */
    /* class,       instance,   title,          tags mask,  isfloating,
       isterminal, noswallow,  monitor */
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, 0, -1, -1},
    {"kitty", NULL, NULL, 0, 0, 1, 0, -1},
    {"Onboard", NULL, NULL, 0, 1, 0, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1}, // xev
    {NULL, "spterm", NULL, SPTAG(0), 1, 1, 0, -1},
    {NULL, "spcalc", NULL, SPTAG(1), 1, 1, 0, -1},
};

/* LAYOUTS */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT                                                           \
  1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol       arrange                         function */
    {"[]=", tile},   /* first entry is default */
    {"TTT", bstack}, /* Master on top, slaves on bottom */

    {"[@]", spiral}, /* Fibonacci spiral */

    {"HHH", grid}, /* A grid of equal sizes */

    {"[D]", deck},    /* Master on left, slaves in monocle-like mode on right */
    {"[M]", monocle}, /* All windows on top of eachother */

    {"|M|", centeredmaster},         /* Master in midle, slaves on sides */
    {">M>", centeredfloatingmaster}, /* Same but master floats */

    {"><>", NULL}, /* no layout function means floating behavior */
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},
#define STACKKEYS(MOD, ACTION)                                                 \
  {MOD, XK_j, ACTION##stack, {.i = INC(+1)}},                                  \
      {MOD, XK_k, ACTION##stack, {.i = INC(-1)}},                              \
      {MOD, XK_v, ACTION##stack, {.i = 0}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

#define STATUSBAR "dwmblocks"

/* COMMANDS */
static const char *termcmd[] = {TERMINAL, NULL};

static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon, "-fn", dmenufont,
                                 "-nb",       bg,    "-nf",    gray3, "-sb",
                                 blue,        "-sf", gray4,    NULL};

/* KEYBINDS */
#include <X11/XF86keysym.h>

static Key keys[] = {
    /* stackeys for windows and tagkeys for workspaces */
    STACKKEYS(MODKEY, focus) STACKKEYS(MODKEY | ShiftMask, push)
        TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
            TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
                TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7) TAGKEYS(XK_9, 8)

    /* modifier                         key                         function
       argument */

    /* { MODKEY,                           XK_Escape,                  , }, */
    /* { MODKEY | ShiftMask,               XK_Escape,                  , }, */
    {MODKEY, XK_bar, tabmode, {-1}},
    /* { MODKEY | ShiftMask,               XK_bar,                     , }, */

    /* 1 - 9 is bound above as tag keys */

    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_plus, spawn,
     SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -44 $(pidof "
           "dwmblocks)")},
    {MODKEY | ShiftMask, XK_plus, spawn,
     SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%+; kill -44 $(pidof "
           "dwmblocks)")},
    /* { MODKEY,                           XK_backslash,               , }, */
    /* { MODKEY | ShiftMask,               XK_backslash,               , }, */

    {MODKEY, XK_Tab, view, {0}},
    /* { MODKEY | ShiftMask,               XK_Tab,                     , }, */
    /* {MODKEY, XK_aring, spawn, SHCMD("scr_def")}, */
    /* {MODKEY | ShiftMask, XK_aring, spawn, SHCMD("scr_home")}, */
    /*{MODKEY, XK_comma, spawn, SHCMD("mpc prev")},*/
    /*{MODKEY | ShiftMask, XK_comma, spawn, SHCMD("mpc seek 0%")},*/
    /*{MODKEY, XK_period, spawn, SHCMD("mpc next")},*/
    /*{MODKEY | ShiftMask, XK_period, spawn, SHCMD("mpc repeat")},*/
    {MODKEY, XK_p, spawn, SHCMD("maimp")},
    {MODKEY | ShiftMask, XK_p, spawn, SHCMD("maim -u -s | ${xclip_cmd}")},
    {MODKEY, XK_y, setlayout, {.v = &layouts[2]}},             /* spiral */
    {MODKEY | ShiftMask, XK_y, setlayout, {.v = &layouts[3]}}, /* grid */
    {MODKEY, XK_f, togglefullscr, {0}},
    /* { MODKEY | ShiftMask,               XK_f,                       , }, */
    /*{MODKEY, XK_g, spawn, SHCMD("maimp")},*/
    /* { MODKEY | ShiftMask,               XK_g,                       , }, */
    /* { MODKEY,                           XK_c,                       , }, */
    /* { MODKEY | ShiftMask,               XK_c,                       , }, */
    {MODKEY, XK_r, spawn, SHCMD(TERMINAL " -e lfub")},
    {MODKEY | ShiftMask, XK_r, spawn, SHCMD(TERMINAL " -e btop")},
    {MODKEY, XK_l, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_l, tagmon, {.i = +1}},
    {MODKEY, XK_apostrophe, togglescratch, {.ui = 0}},
    {MODKEY | ShiftMask, XK_apostrophe, togglescratch, {.ui = 1}},
    /* { MODKEY,                           XK_asciitilde,              , }, */
    /* { MODKEY | ShiftMask,               XK_asciitilde,              , }, */

    {MODKEY, XK_a, togglegaps, {0}},
    {MODKEY | ShiftMask, XK_a, defaultgaps, {0}},
    {MODKEY, XK_o, incnmaster, {.i = +1}},
    {MODKEY | ShiftMask, XK_o, incnmaster, {.i = -1}},
    /* { MODKEY,                           XK_e,                       , }, */
    /* { MODKEY | ShiftMask,               XK_e,                       , }, */
    {MODKEY, XK_u, setlayout, {.v = &layouts[4]}},             /* deck */
    {MODKEY | ShiftMask, XK_u, setlayout, {.v = &layouts[5]}}, /* monocle */
    {MODKEY, XK_i, setlayout, {.v = &layouts[6]}}, /* centeredmaster */
    {MODKEY | ShiftMask,
     XK_i,
     setlayout,
     {.v = &layouts[7]}}, /* centeredfloatingmaster */
    {MODKEY, XK_d, spawn, SHCMD("rofi -show drun")},
    /* { MODKEY | ShiftMask,               XK_d,                       , }, */
    {MODKEY, XK_h, focusmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_h, tagmon, {.i = -1}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},             /* tile */
    {MODKEY | ShiftMask, XK_t, setlayout, {.v = &layouts[1]}}, /* bstack */
    /* { MODKEY,                           XK_n,                       , }, */
    // TODO: Doesn't work
    {MODKEY | ShiftMask, XK_n, spawn, SHCMD(TERMINAL " -e newsboat")},
    {MODKEY, XK_s, togglesticky, {0}},
    {MODKEY | ShiftMask, XK_s, togglesmartgaps, {0}},
    {MODKEY, XK_minus, spawn,
     SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -44 $(pidof "
           "dwmblocks)")},
    {MODKEY | ShiftMask, XK_minus, spawn,
     SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%-; kill -44 $(pidof "
           "dwmblocks)")},
    /* { MODKEY,                           XK_less,                    spawn,
       SHCMD("setxkbmap -model pc105 -layout dk -option caps:escape")}, */
    /* { MODKEY | ShiftMask,               XK_less,                    spawn,
       SHCMD("setxkbmap -model pc105 -layout dk -variant dvorak -option
       caps:escape")}, */

    {MODKEY, XK_oslash, spawn, SHCMD("mpc seek +10")},
    {MODKEY | ShiftMask, XK_oslash, spawn, SHCMD("mpc seek +60")},
    {MODKEY, XK_ae, spawn, SHCMD("mpc seek -10")},
    {MODKEY | ShiftMask, XK_ae, spawn, SHCMD("mpc seek -60")},
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY | ShiftMask, XK_q, spawn, SHCMD("pwrmenu")},

    /* j and k is bound above as stack keys */

    {MODKEY, XK_x, incrgaps, {.i = -5}},
    /* { MODKEY | ShiftMask,               XK_x,                       , }, */
    {MODKEY, XK_b, togglebar, {0}},
    /* { MODKEY | ShiftMask,               XK_b,                       , }, */
    /* { MODKEY,                           XK_m,                       , }, */
    {MODKEY | ShiftMask, XK_m, spawn,
     SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof "
           "dwmblocks)")},
    {MODKEY, XK_w, spawn, SHCMD("$BROWSER")},
    {MODKEY | ShiftMask, XK_w, spawn, SHCMD(TERMINAL " -e sudo nmtui")},

    /* v is bound above as stack key */

    {MODKEY, XK_z, incrgaps, {.i = +5}},
    /* { MODKEY | ShiftMask,               XK_z,                       , }, */
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    /* { MODKEY | ShiftMask,               XK_Return,                  , }, */
    /* { MODKEY,                           XK_BackSpace,               , }, */
    /* { MODKEY | ShiftMask,               XK_BackSpace,               , }, */

    {MODKEY, XK_space, zoom, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {
        MODKEY,
        XK_Left,
        setmfact,
        {.f = -0.05},
    },
    /* { MODKEY | ShiftMask,               XK_Left,                    , }, */
    {
        MODKEY,
        XK_Right,
        setmfact,
        {.f = +0.05},
    },
    /* { MODKEY | ShiftMask,               XK_Right,                   , }, */
    {
        MODKEY,
        XK_Up,
        setcfact,
        {.f = +0.25},
    },
    /* { MODKEY | ShiftMask,               XK_Up,                      , }, */
    {
        MODKEY,
        XK_Down,
        setcfact,
        {.f = -0.25},
    },
    /* { MODKEY | ShiftMask,               XK_Down,                    , }, */

    {ShiftMask, XK_Print, spawn,
     SHCMD("maim -u -s | ${xclip_cmd}")},
    {0, XK_Print, spawn, SHCMD("maimp")},

    {0, XF86XK_AudioMute, spawn,
     SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof "
           "dwmblocks)")},
    {0, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -44 $(pidof "
           "dwmblocks)")},
    {0, XF86XK_AudioLowerVolume, spawn,
     SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -44 $(pidof "
           "dwmblocks)")},
    {0, XF86XK_AudioMicMute, spawn,
     SHCMD("wpctl set-source-mute @DEFAULT_SOURCE@ toggle")},
    {0, XF86XK_Calculator, spawn, SHCMD(TERMINAL " e bc -l")},
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("sudo xbacklight -inc 10")},
    {0, XF86XK_MonBrightnessDown, spawn, SHCMD("sudo xbacklight -dec 10")},
};

/* BUTTON DEFINITIONS */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click         event mask  button      function        argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkStatusText, 0, Button1, sigstatusbar, {.i = 1}},
    {ClkStatusText, 0, Button2, sigstatusbar, {.i = 2}},
    {ClkStatusText, 0, Button3, sigstatusbar, {.i = 3}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
    {ClkTabBar, 0, Button1, focuswin, {0}},
};
