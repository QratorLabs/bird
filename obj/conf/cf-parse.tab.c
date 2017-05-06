/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         cf_parse
#define yylex           cf_lex
#define yyerror         cf_error
#define yydebug         cf_debug
#define yynerrs         cf_nerrs

#define yylval          cf_lval
#define yychar          cf_char

/* Copy the first part of user declarations.  */
#line 1 "cf-parse.y" /* yacc.c:339  */

/* Headers from ../../conf/confbase.Y */

#define PARSER 1

#include "nest/bird.h"
#include "conf/conf.h"
#include "lib/resource.h"
#include "lib/socket.h"
#include "lib/timer.h"
#include "lib/string.h"
#include "nest/protocol.h"
#include "nest/iface.h"
#include "nest/route.h"
#include "nest/cli.h"
#include "filter/filter.h"

/* FIXME: Turn on YYERROR_VERBOSE and work around lots of bison bugs? */

/* Defines from ../../conf/confbase.Y */

static void
check_u16(unsigned val)
{
  if (val > 0xFFFF)
    cf_error("Value %d out of range (0-65535)", val);
}

/* Headers from ../../sysdep/unix/config.Y */

#include "lib/unix.h"
#include <stdio.h>

/* Headers from ../../sysdep/unix/krt.Y */

#include "lib/krt.h"

/* Defines from ../../sysdep/unix/krt.Y */

#define THIS_KRT ((struct krt_config *) this_proto)
#define THIS_KIF ((struct kif_config *) this_proto)

/* Headers from ../../sysdep/linux/netlink.Y */

/* Headers from ../../nest/config.Y */

#include "nest/rt-dev.h"
#include "nest/password.h"
#include "nest/cmds.h"
#include "lib/lists.h"

/* Defines from ../../nest/config.Y */

static struct proto_config *this_proto;
static struct iface_patt *this_ipatt;
static struct iface_patt_node *this_ipn;
static struct roa_table_config *this_roa_table;
static list *this_p_list;
static struct password_item *this_p_item;
static int password_id;

static void
iface_patt_check(void)
{
  struct iface_patt_node *pn;

  WALK_LIST(pn, this_ipatt->ipn_list)
    if (!pn->pattern || pn->pxlen)
      cf_error("Interface name/mask expected, not IP prefix");
}


static inline void
reset_passwords(void)
{
  this_p_list = NULL;
}

static inline list *
get_passwords(void)
{
  list *rv = this_p_list;
  this_p_list = NULL;
  return rv;
}

#define DIRECT_CFG ((struct rt_dev_config *) this_proto)

/* Headers from ../../filter/config.Y */

/* Defines from ../../filter/config.Y */

#define P(a,b) ((a << 8) | b)

static inline u32 pair(u32 a, u32 b) { return (a << 16) | b; }
static inline u32 pair_a(u32 p) { return p >> 16; }
static inline u32 pair_b(u32 p) { return p & 0xFFFF; }


/*
 * Sets and their items are during parsing handled as lists, linked
 * through left ptr. The first item in a list also contains a pointer
 * to the last item in a list (right ptr). For convenience, even items
 * are handled as one-item lists. Lists are merged by f_merge_items().
 */
static int
f_valid_set_type(int type)
{
  switch (type)
  {
  case T_INT:
  case T_PAIR:
  case T_QUAD:
  case T_ENUM:
  case T_IP:
  case T_EC:
    return 1;

  default:
    return 0;
  }
}

static inline struct f_tree *
f_new_item(struct f_val from, struct f_val to)
{
  struct f_tree *t = f_new_tree();
  t->right = t;
  t->from = from;
  t->to = to;
  return t;
}

static inline struct f_tree *
f_merge_items(struct f_tree *a, struct f_tree *b)
{
  if (!a) return b;
  a->right->left = b;
  a->right = b->right;
  b->right = NULL;
  return a;
}

static inline struct f_tree *
f_new_pair_item(int fa, int ta, int fb, int tb)
{
  struct f_tree *t = f_new_tree();
  t->right = t;
  t->from.type = t->to.type = T_PAIR;
  t->from.val.i = pair(fa, fb);
  t->to.val.i = pair(ta, tb);
  return t;
}

static inline struct f_tree *
f_new_pair_set(int fa, int ta, int fb, int tb)
{
  struct f_tree *lst = NULL;
  int i;

  if ((fa == ta) || ((fb == 0) && (tb == 0xFFFF)))
    return f_new_pair_item(fa, ta, fb, tb);
  
  if ((ta < fa) || (tb < fb))
    cf_error( "From value cannot be higher that To value in pair sets");

  for (i = fa; i <= ta; i++)
    lst = f_merge_items(lst, f_new_pair_item(i, i, fb, tb));

  return lst;
}

#define EC_ALL 0xFFFFFFFF

static struct f_tree *
f_new_ec_item(u32 kind, u32 ipv4_used, u32 key, u32 vf, u32 vt)
{
  u64 fm, to;

  if (ipv4_used || (key >= 0x10000)) {
    check_u16(vf);
    if (vt == EC_ALL)
      vt = 0xFFFF;
    else
      check_u16(vt);
  }

  if (kind == EC_GENERIC) {
    fm = ec_generic(key, vf);
    to = ec_generic(key, vt);
  }
  else if (ipv4_used) {
    fm = ec_ip4(kind, key, vf);
    to = ec_ip4(kind, key, vt);
  }
  else if (key < 0x10000) {
    fm = ec_as2(kind, key, vf);
    to = ec_as2(kind, key, vt);
  }
  else {
    fm = ec_as4(kind, key, vf);
    to = ec_as4(kind, key, vt);
  }

  struct f_tree *t = f_new_tree();
  t->right = t;
  t->from.type = t->to.type = T_EC;
  t->from.val.ec = fm;
  t->to.val.ec = to;
  return t;
}

static inline struct f_inst *
f_generate_empty(struct f_inst *dyn)
{ 
  struct f_inst *e = f_new_inst();
  e->code = 'E';

  switch (dyn->aux & EAF_TYPE_MASK) {
    case EAF_TYPE_AS_PATH:
      e->aux = T_PATH;
      break;
    case EAF_TYPE_INT_SET:
      e->aux = T_CLIST;
      break;
    case EAF_TYPE_EC_SET:
      e->aux = T_ECLIST;
      break;
    default:
      cf_error("Can't empty that attribute");
  }

  dyn->code = P('e','S');
  dyn->a1.p = e;
  return dyn;
}


static inline struct f_inst *
f_generate_dpair(struct f_inst *t1, struct f_inst *t2)
{
  struct f_inst *rv;

  if ((t1->code == 'c') && (t2->code == 'c')) {
    if ((t1->aux != T_INT) || (t2->aux != T_INT))
      cf_error( "Can't operate with value of non-integer type in pair constructor");

    check_u16(t1->a2.i);
    check_u16(t2->a2.i);

    rv = f_new_inst();
    rv->code = 'c';
    rv->aux = T_PAIR;
    rv->a2.i = pair(t1->a2.i, t2->a2.i);
  }
  else {
    rv = f_new_inst();
    rv->code = P('m', 'p');
    rv->a1.p = t1;
    rv->a2.p = t2;
  }

  return rv;
}

static inline struct f_inst *
f_generate_ec(u16 kind, struct f_inst *tk, struct f_inst *tv)
{
  struct f_inst *rv;
  int c1 = 0, c2 = 0, ipv4_used = 0;
  u32 key = 0, val2 = 0;

  if (tk->code == 'c') {
    c1 = 1;

    if (tk->aux == T_INT) {
      ipv4_used = 0; key = tk->a2.i;
    }
    else if (tk->aux == T_QUAD) {
      ipv4_used = 1; key = tk->a2.i;
    }
    else
      cf_error("Can't operate with key of non-integer/IPv4 type in EC constructor");
  }

#ifndef IPV6
  /* IP->Quad implicit conversion */
  else if (tk->code == 'C') {
    c1 = 1;
    struct f_val *val = tk->a1.p;

    if (val->type == T_INT) {
      ipv4_used = 0; key = val->val.i;
    }
    else if (val->type == T_QUAD) {
      ipv4_used = 1; key = val->val.i;
    }
    else if (val->type == T_IP) {
      ipv4_used = 1; key = ipa_to_u32(val->val.px.ip);
    }
    else
      cf_error("Can't operate with key of non-integer/IPv4 type in EC constructor");
  }
#endif

  if (tv->code == 'c') {
    if (tv->aux != T_INT)
      cf_error("Can't operate with value of non-integer type in EC constructor");
    c2 = 1;
    val2 = tv->a2.i;
  }

  if (c1 && c2) {
    u64 ec;
  
    if (kind == EC_GENERIC) {
      ec = ec_generic(key, val2);
    }
    else if (ipv4_used) {
      check_u16(val2);
      ec = ec_ip4(kind, key, val2);
    }
    else if (key < 0x10000) {
      ec = ec_as2(kind, key, val2);
    }
    else {
      check_u16(val2);
      ec = ec_as4(kind, key, val2);
    }

    NEW_F_VAL;
    rv = f_new_inst();
    rv->code = 'C';
    rv->a1.p = val;    
    val->type = T_EC;
    val->val.ec = ec;
  }
  else {
    rv = f_new_inst();
    rv->code = P('m','c');
    rv->aux = kind;
    rv->a1.p = tk;
    rv->a2.p = tv;
  }

  return rv;
}



/* Headers from ../../proto/bfd/config.Y */

#include "proto/bfd/bfd.h"

/* Defines from ../../proto/bfd/config.Y */

#define BFD_CFG ((struct bfd_config *) this_proto)
#define BFD_IFACE ((struct bfd_iface_config *) this_ipatt)
#define BFD_NEIGHBOR this_bfd_neighbor

static struct bfd_neighbor *this_bfd_neighbor;

extern struct bfd_config *bfd_cf;

/* Headers from ../../proto/bgp/config.Y */

#include "proto/bgp/bgp.h"

/* Defines from ../../proto/bgp/config.Y */

#define BGP_CFG ((struct bgp_config *) this_proto)

/* Headers from ../../proto/ospf/config.Y */

#include "proto/ospf/ospf.h"

/* Defines from ../../proto/ospf/config.Y */

#define OSPF_CFG ((struct ospf_config *) this_proto)
#define OSPF_PATT ((struct ospf_iface_patt *) this_ipatt)

static struct ospf_area_config *this_area;
static struct nbma_node *this_nbma;
static list *this_nets;
static struct area_net_config *this_pref;
static struct ospf_stubnet_config *this_stubnet;

static inline int ospf_cfg_is_v2(void) { return OSPF_CFG->ospf2; }
static inline int ospf_cfg_is_v3(void) { return ! OSPF_CFG->ospf2; }

static void
ospf_iface_finish(void)
{
  struct ospf_iface_patt *ip = OSPF_PATT;

  if (ip->deadint == 0)
    ip->deadint = ip->deadc * ip->helloint;

  if (ip->waitint == 0)
    ip->waitint = ip->deadc * ip->helloint;

  ip->passwords = get_passwords();

  if ((ip->autype == OSPF_AUTH_CRYPT) && (ip->helloint < 5))
    log(L_WARN "Hello or poll interval less that 5 makes cryptographic authenication prone to replay attacks");

  if ((ip->autype == OSPF_AUTH_NONE) && (ip->passwords != NULL))
    log(L_WARN "Password option without authentication option does not make sense");
}

static void
ospf_area_finish(void)
{
  if ((this_area->areaid == 0) && (this_area->type != OPT_E))
    cf_error("Backbone area cannot be stub/NSSA");

  if (this_area->summary && (this_area->type == OPT_E))
    cf_error("Only stub/NSSA areas can use summary propagation");

  if (this_area->default_nssa && ((this_area->type != OPT_N) || ! this_area->summary))
    cf_error("Only NSSA areas with summary propagation can use NSSA default route");

  if ((this_area->default_cost & LSA_EXT3_EBIT) && ! this_area->default_nssa)
    cf_error("Only NSSA default route can use type 2 metric");
}

static void
ospf_proto_finish(void)
{
  struct ospf_config *cf = OSPF_CFG;

  if (EMPTY_LIST(cf->area_list))
    cf_error( "No configured areas in OSPF");

  int areano = 0;
  int backbone = 0;
  int nssa = 0;
  struct ospf_area_config *ac;
  WALK_LIST(ac, cf->area_list)
  {
    areano++;
    if (ac->areaid == 0)
      backbone = 1;
    if (ac->type == OPT_N)
      nssa = 1;
  }

  cf->abr = areano > 1;

  /* Route export or NSSA translation (RFC 3101 3.1) */
  cf->asbr = (this_proto->out_filter != FILTER_REJECT) || (nssa && cf->abr);

  if (cf->abr && !backbone)
  {
    struct ospf_area_config *ac = cfg_allocz(sizeof(struct ospf_area_config));
    ac->type = OPT_E; /* Backbone is non-stub */
    add_head(&cf->area_list, NODE ac);
    init_list(&ac->patt_list);
    init_list(&ac->net_list);
    init_list(&ac->enet_list);
    init_list(&ac->stubnet_list);
  }

  if (!cf->abr && !EMPTY_LIST(cf->vlink_list))
    cf_error("Vlinks cannot be used on single area router");

  if (cf->asbr && (areano == 1) && (this_area->type == 0))
    cf_error("ASBR must be in non-stub area");
}

static inline void
ospf_check_defcost(int cost)
{
  if ((cost <= 0) || (cost >= LSINFINITY))
   cf_error("Default cost must be in range 1-%d", LSINFINITY-1);
}

static inline void
ospf_check_auth(void)
{
  if (ospf_cfg_is_v3())
    cf_error("Authentication not supported in OSPFv3");
}


/* Headers from ../../proto/pipe/config.Y */

#include "proto/pipe/pipe.h"

/* Defines from ../../proto/pipe/config.Y */

#define PIPE_CFG ((struct pipe_config *) this_proto)

/* Headers from ../../proto/rip/config.Y */

#include "proto/rip/rip.h"
#include "nest/iface.h"

/* Defines from ../../proto/rip/config.Y */

#define RIP_CFG ((struct rip_proto_config *) this_proto)
#define RIP_IPATT ((struct rip_patt *) this_ipatt)

#ifdef IPV6
#define RIP_DEFAULT_TTL_SECURITY 2
#else
#define RIP_DEFAULT_TTL_SECURITY 0
#endif

/* Headers from ../../proto/static/config.Y */

#include "proto/static/static.h"

/* Defines from ../../proto/static/config.Y */

#define STATIC_CFG ((struct static_config *) this_proto)
static struct static_route *this_srt, *this_srt_nh, *last_srt_nh;
static struct f_inst **this_srt_last_cmd;

static void
static_route_finish(void)
{
  struct static_route *r;

  /* Update undefined use_bfd entries in multipath nexthops */
  if (this_srt->dest == RTD_MULTIPATH)
    for (r = this_srt->mp_next; r; r = r->mp_next)
      if (r->use_bfd < 0)
        r->use_bfd = this_srt->use_bfd;
}


#line 607 "cf-parse.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "cf-parse.tab.h".  */
#ifndef YY_CF_CF_PARSE_TAB_H_INCLUDED
# define YY_CF_CF_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int cf_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END = 258,
    CLI_MARKER = 259,
    INVALID_TOKEN = 260,
    ELSECOL = 261,
    DDOT = 262,
    GEQ = 263,
    LEQ = 264,
    NEQ = 265,
    AND = 266,
    OR = 267,
    PO = 268,
    PC = 269,
    NUM = 270,
    ENUM = 271,
    RTRID = 272,
    IPA = 273,
    SYM = 274,
    TEXT = 275,
    PREFIX_DUMMY = 276,
    DEFINE = 277,
    ON = 278,
    OFF = 279,
    YES = 280,
    NO = 281,
    S = 282,
    MS = 283,
    US = 284,
    PORT = 285,
    LOG = 286,
    SYSLOG = 287,
    ALL = 288,
    DEBUG = 289,
    TRACE = 290,
    INFO = 291,
    REMOTE = 292,
    WARNING = 293,
    ERROR = 294,
    AUTH = 295,
    FATAL = 296,
    BUG = 297,
    STDERR = 298,
    SOFT = 299,
    TIMEFORMAT = 300,
    ISO = 301,
    OLD = 302,
    SHORT = 303,
    LONG = 304,
    BASE = 305,
    NAME = 306,
    CONFIRM = 307,
    UNDO = 308,
    CHECK = 309,
    TIMEOUT = 310,
    LATENCY = 311,
    LIMIT = 312,
    WATCHDOG = 313,
    CONFIGURE = 314,
    DOWN = 315,
    KERNEL = 316,
    PERSIST = 317,
    SCAN = 318,
    TIME = 319,
    LEARN = 320,
    DEVICE = 321,
    ROUTES = 322,
    GRACEFUL = 323,
    RESTART = 324,
    KRT_SOURCE = 325,
    KRT_METRIC = 326,
    MERGE = 327,
    PATHS = 328,
    TABLE = 329,
    KRT_PREFSRC = 330,
    KRT_REALM = 331,
    KRT_MTU = 332,
    KRT_WINDOW = 333,
    KRT_RTT = 334,
    KRT_RTTVAR = 335,
    KRT_SSTRESH = 336,
    KRT_CWND = 337,
    KRT_ADVMSS = 338,
    KRT_REORDERING = 339,
    KRT_HOPLIMIT = 340,
    KRT_INITCWND = 341,
    KRT_RTO_MIN = 342,
    KRT_INITRWND = 343,
    KRT_QUICKACK = 344,
    KRT_LOCK_MTU = 345,
    KRT_LOCK_WINDOW = 346,
    KRT_LOCK_RTT = 347,
    KRT_LOCK_RTTVAR = 348,
    KRT_LOCK_SSTRESH = 349,
    KRT_LOCK_CWND = 350,
    KRT_LOCK_ADVMSS = 351,
    KRT_LOCK_REORDERING = 352,
    KRT_LOCK_HOPLIMIT = 353,
    KRT_LOCK_RTO_MIN = 354,
    KRT_FEATURE_ECN = 355,
    KRT_FEATURE_ALLFRAG = 356,
    ROUTER = 357,
    ID = 358,
    PROTOCOL = 359,
    TEMPLATE = 360,
    PREFERENCE = 361,
    DISABLED = 362,
    DIRECT = 363,
    INTERFACE = 364,
    IMPORT = 365,
    EXPORT = 366,
    FILTER = 367,
    NONE = 368,
    STATES = 369,
    FILTERS = 370,
    RECEIVE = 371,
    ACTION = 372,
    WARN = 373,
    BLOCK = 374,
    DISABLE = 375,
    KEEP = 376,
    FILTERED = 377,
    PASSWORD = 378,
    FROM = 379,
    PASSIVE = 380,
    TO = 381,
    EVENTS = 382,
    PACKETS = 383,
    PROTOCOLS = 384,
    INTERFACES = 385,
    PRIMARY = 386,
    STATS = 387,
    COUNT = 388,
    FOR = 389,
    COMMANDS = 390,
    PREEXPORT = 391,
    NOEXPORT = 392,
    GENERATE = 393,
    ROA = 394,
    LISTEN = 395,
    BGP = 396,
    V6ONLY = 397,
    DUAL = 398,
    ADDRESS = 399,
    PASSWORDS = 400,
    DESCRIPTION = 401,
    SORTED = 402,
    RELOAD = 403,
    IN = 404,
    OUT = 405,
    MRTDUMP = 406,
    MESSAGES = 407,
    RESTRICT = 408,
    MEMORY = 409,
    IGP_METRIC = 410,
    CLASS = 411,
    DSCP = 412,
    WAIT = 413,
    MAX = 414,
    FLUSH = 415,
    AS = 416,
    SHOW = 417,
    STATUS = 418,
    SUMMARY = 419,
    ROUTE = 420,
    SYMBOLS = 421,
    ADD = 422,
    DELETE = 423,
    DUMP = 424,
    RESOURCES = 425,
    SOCKETS = 426,
    NEIGHBORS = 427,
    ATTRIBUTES = 428,
    EVAL = 429,
    ECHO = 430,
    ENABLE = 431,
    FUNCTION = 432,
    PRINT = 433,
    PRINTN = 434,
    UNSET = 435,
    RETURN = 436,
    ACCEPT = 437,
    REJECT = 438,
    QUITBIRD = 439,
    INT = 440,
    BOOL = 441,
    IP = 442,
    PREFIX = 443,
    PAIR = 444,
    QUAD = 445,
    EC = 446,
    SET = 447,
    STRING = 448,
    BGPMASK = 449,
    BGPPATH = 450,
    CLIST = 451,
    ECLIST = 452,
    IF = 453,
    THEN = 454,
    ELSE = 455,
    CASE = 456,
    TRUE = 457,
    FALSE = 458,
    RT = 459,
    RO = 460,
    UNKNOWN = 461,
    GENERIC = 462,
    GW = 463,
    NET = 464,
    MASK = 465,
    PROTO = 466,
    SOURCE = 467,
    SCOPE = 468,
    CAST = 469,
    DEST = 470,
    IFNAME = 471,
    IFINDEX = 472,
    LEN = 473,
    DEFINED = 474,
    CONTAINS = 475,
    RESET = 476,
    PREPEND = 477,
    FIRST = 478,
    LAST = 479,
    MATCH = 480,
    ROA_CHECK = 481,
    EMPTY = 482,
    WHERE = 483,
    ROLE_MAP = 484,
    NET_PREFIX = 485,
    PREFIX_ROLE = 486,
    BFD = 487,
    MIN = 488,
    IDLE = 489,
    RX = 490,
    TX = 491,
    INTERVAL = 492,
    MULTIPLIER = 493,
    MULTIHOP = 494,
    NEIGHBOR = 495,
    DEV = 496,
    LOCAL = 497,
    SESSIONS = 498,
    HOLD = 499,
    CONNECT = 500,
    RETRY = 501,
    KEEPALIVE = 502,
    STARTUP = 503,
    VIA = 504,
    NEXT = 505,
    HOP = 506,
    SELF = 507,
    DEFAULT = 508,
    PATH = 509,
    METRIC = 510,
    START = 511,
    DELAY = 512,
    FORGET = 513,
    AFTER = 514,
    BGP_PATH = 515,
    BGP_LOCAL_PREF = 516,
    BGP_MED = 517,
    BGP_ORIGIN = 518,
    BGP_NEXT_HOP = 519,
    BGP_ATOMIC_AGGR = 520,
    BGP_AGGREGATOR = 521,
    BGP_COMMUNITY = 522,
    BGP_EXT_COMMUNITY = 523,
    RR = 524,
    RS = 525,
    CLIENT = 526,
    CLUSTER = 527,
    AS4 = 528,
    ADVERTISE = 529,
    IPV4 = 530,
    CAPABILITIES = 531,
    PREFER = 532,
    OLDER = 533,
    MISSING = 534,
    LLADDR = 535,
    DROP = 536,
    IGNORE = 537,
    REFRESH = 538,
    INTERPRET = 539,
    COMMUNITIES = 540,
    BGP_ORIGINATOR_ID = 541,
    BGP_CLUSTER_LIST = 542,
    IGP = 543,
    GATEWAY = 544,
    RECURSIVE = 545,
    MED = 546,
    TTL = 547,
    SECURITY = 548,
    DETERMINISTIC = 549,
    SECONDARY = 550,
    ALLOW = 551,
    AWARE = 552,
    LINK = 553,
    EXTENDED = 554,
    ROLE = 555,
    PEER = 556,
    PROVIDER = 557,
    CUSTOMER = 558,
    INTERNAL = 559,
    COMPLEX = 560,
    STRICT_MODE = 561,
    USE = 562,
    OSPF = 563,
    AREA = 564,
    OSPF_METRIC1 = 565,
    OSPF_METRIC2 = 566,
    OSPF_TAG = 567,
    OSPF_ROUTER_ID = 568,
    RFC1583COMPAT = 569,
    STUB = 570,
    TICK = 571,
    COST = 572,
    COST2 = 573,
    RETRANSMIT = 574,
    HELLO = 575,
    TRANSMIT = 576,
    PRIORITY = 577,
    DEAD = 578,
    TYPE = 579,
    BROADCAST = 580,
    BCAST = 581,
    NONBROADCAST = 582,
    NBMA = 583,
    POINTOPOINT = 584,
    PTP = 585,
    POINTOMULTIPOINT = 586,
    PTMP = 587,
    SIMPLE = 588,
    AUTHENTICATION = 589,
    STRICT = 590,
    CRYPTOGRAPHIC = 591,
    ELIGIBLE = 592,
    POLL = 593,
    NETWORKS = 594,
    HIDDEN = 595,
    VIRTUAL = 596,
    ONLY = 597,
    BUFFER = 598,
    LARGE = 599,
    NORMAL = 600,
    STUBNET = 601,
    TAG = 602,
    EXTERNAL = 603,
    LSADB = 604,
    ECMP = 605,
    WEIGHT = 606,
    NSSA = 607,
    TRANSLATOR = 608,
    STABILITY = 609,
    GLOBAL = 610,
    LSID = 611,
    INSTANCE = 612,
    REAL = 613,
    NETMASK = 614,
    LENGTH = 615,
    LSA = 616,
    SUPPRESSION = 617,
    TOPOLOGY = 618,
    STATE = 619,
    PIPE = 620,
    MODE = 621,
    OPAQUE = 622,
    TRANSPARENT = 623,
    RIP = 624,
    INFINITY = 625,
    PERIOD = 626,
    GARBAGE = 627,
    MULTICAST = 628,
    QUIET = 629,
    NOLISTEN = 630,
    VERSION1 = 631,
    PLAINTEXT = 632,
    MD5 = 633,
    HONOR = 634,
    NEVER = 635,
    ALWAYS = 636,
    RIP_METRIC = 637,
    RIP_TAG = 638,
    STATIC = 639,
    PROHIBIT = 640,
    MULTIPATH = 641,
    BLACKHOLE = 642,
    UNREACHABLE = 643
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 536 "cf-parse.y" /* yacc.c:355  */

  int i;
  u32 i32;
  ip_addr a;
  struct symbol *s;
  char *t;
  struct rtable_config *r;
  struct f_inst *x;
  struct role_map *rm;
  struct filter *f;
  struct f_tree *e;
  struct f_trie *trie;
  struct f_val v;
  struct f_path_mask *h;
  struct password_item *p;
  struct rt_show_data *ra;
  struct roa_show_data *ro;
  struct sym_show_data *sd;
  struct lsadb_show_data *ld;
  struct iface *iface;
  struct roa_table *rot;
  void *g;
  bird_clock_t time;
  struct prefix px;
  struct proto_spec ps;
  struct timeformat *tf;

#line 1064 "cf-parse.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE cf_lval;

int cf_parse (void);

#endif /* !YY_CF_CF_PARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1081 "cf-parse.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3062

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  410
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  267
/* YYNRULES -- Number of rules.  */
#define YYNRULES  906
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1692

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   643

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,     2,     2,    30,     2,     2,
     401,   402,    28,    26,   406,    27,    32,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   403,   400,
      23,    22,    24,   407,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   408,     2,   409,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   404,     2,   405,    25,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   728,   728,   729,   732,   734,   742,   751,   752,   753,
     760,   761,   762,   770,   771,   772,   773,   774,   775,   781,
     782,   789,   790,   794,   801,   802,   806,   810,   817,   825,
     826,   833,   834,   840,   849,   850,   854,   859,   860,   864,
     865,   869,   870,   874,   875,   876,   877,   878,   879,   880,
     881,   882,   888,   889,   899,   900,   901,   902,   905,   906,
     907,   908,   909,   910,   914,   920,   921,   922,   923,   931,
     934,   941,   944,   947,   950,   954,   955,   959,   960,   961,
     969,   974,   975,   979,   986,   987,   988,   989,   995,  1000,
    1004,  1018,  1036,  1037,  1041,  1042,  1043,  1044,  1051,  1065,
    1067,  1069,  1073,  1074,  1075,  1076,  1081,  1087,  1088,  1092,
    1100,  1104,  1106,  1112,  1113,  1120,  1121,  1125,  1131,  1135,
    1144,  1153,  1155,  1159,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1170,  1174,  1175,  1176,  1177,  1181,
    1182,  1183,  1184,  1185,  1189,  1195,  1199,  1207,  1208,  1216,
    1224,  1225,  1226,  1230,  1231,  1235,  1240,  1241,  1245,  1247,
    1255,  1259,  1260,  1266,  1273,  1274,  1275,  1279,  1287,  1293,
    1294,  1295,  1299,  1300,  1304,  1305,  1306,  1307,  1308,  1309,
    1315,  1316,  1317,  1321,  1322,  1326,  1327,  1333,  1334,  1337,
    1339,  1343,  1344,  1348,  1366,  1367,  1368,  1369,  1370,  1371,
    1379,  1382,  1385,  1388,  1392,  1393,  1396,  1399,  1403,  1407,
    1412,  1418,  1425,  1430,  1435,  1440,  1444,  1448,  1452,  1461,
    1469,  1473,  1480,  1481,  1482,  1487,  1491,  1498,  1505,  1509,
    1517,  1518,  1519,  1524,  1528,  1531,  1532,  1533,  1534,  1535,
    1536,  1537,  1538,  1543,  1548,  1556,  1563,  1570,  1578,  1580,
    1582,  1584,  1586,  1588,  1590,  1592,  1595,  1599,  1605,  1606,
    1607,  1611,  1612,  1618,  1620,  1622,  1624,  1626,  1628,  1632,
    1636,  1639,  1643,  1644,  1645,  1649,  1650,  1651,  1659,  1659,
    1669,  1669,  1679,  1683,  1684,  1685,  1686,  1687,  1688,  1689,
    1690,  1691,  1692,  1693,  1694,  1695,  1716,  1727,  1728,  1735,
    1736,  1743,  1752,  1753,  1754,  1755,  1759,  1779,  1783,  1787,
    1791,  1795,  1819,  1820,  1824,  1837,  1837,  1853,  1854,  1857,
    1858,  1862,  1865,  1874,  1887,  1888,  1889,  1890,  1891,  1895,
    1903,  1904,  1905,  1906,  1907,  1911,  1914,  1915,  1916,  1920,
    1923,  1932,  1935,  1936,  1937,  1938,  1942,  1943,  1944,  1948,
    1949,  1950,  1951,  1955,  1956,  1957,  1958,  1962,  1963,  1967,
    1968,  1972,  1979,  1980,  1981,  1982,  1989,  1990,  1993,  1994,
    2001,  2013,  2014,  2018,  2019,  2023,  2024,  2025,  2026,  2027,
    2031,  2032,  2033,  2037,  2038,  2039,  2040,  2041,  2042,  2043,
    2044,  2045,  2046,  2047,  2051,  2052,  2062,  2066,  2089,  2103,
    2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,  2112,  2116,
    2117,  2118,  2119,  2120,  2121,  2122,  2123,  2124,  2125,  2126,
    2127,  2128,  2129,  2130,  2131,  2133,  2134,  2135,  2137,  2139,
    2140,  2142,  2144,  2145,  2146,  2147,  2148,  2158,  2159,  2160,
    2161,  2162,  2163,  2164,  2166,  2167,  2172,  2195,  2196,  2197,
    2198,  2199,  2200,  2204,  2207,  2208,  2209,  2217,  2224,  2233,
    2234,  2238,  2244,  2254,  2263,  2273,  2279,  2284,  2291,  2296,
    2302,  2303,  2304,  2312,  2313,  2314,  2315,  2316,  2322,  2334,
    2335,  2336,  2337,  2340,  2342,  2346,  2350,  2361,  2362,  2363,
    2364,  2365,  2366,  2369,  2371,  2374,  2376,  2379,  2382,  2387,
    2388,  2389,  2393,  2394,  2398,  2399,  2402,  2421,  2427,  2454,
    2456,  2457,  2461,  2462,  2463,  2464,  2465,  2466,  2472,  2473,
    2474,  2475,  2476,  2477,  2478,  2479,  2480,  2481,  2482,  2483,
    2484,  2485,  2486,  2487,  2488,  2489,  2490,  2491,  2492,  2493,
    2494,  2495,  2496,  2497,  2498,  2499,  2500,  2501,  2502,  2503,
    2504,  2505,  2506,  2507,  2508,  2509,  2515,  2516,  2517,  2518,
    2519,  2520,  2521,  2522,  2523,  2524,  2525,  2526,  2527,  2528,
    2529,  2530,  2531,  2532,  2543,  2553,  2554,  2558,  2559,  2560,
    2561,  2562,  2563,  2564,  2565,  2566,  2569,  2584,  2587,  2589,
    2593,  2594,  2595,  2596,  2597,  2598,  2599,  2600,  2601,  2602,
    2602,  2603,  2603,  2604,  2605,  2606,  2610,  2611,  2615,  2623,
    2625,  2629,  2630,  2631,  2635,  2636,  2639,  2641,  2644,  2645,
    2646,  2647,  2648,  2649,  2650,  2651,  2652,  2653,  2654,  2657,
    2677,  2678,  2679,  2680,  2681,  2682,  2683,  2684,  2685,  2686,
    2687,  2688,  2689,  2690,  2691,  2692,  2693,  2694,  2695,  2696,
    2697,  2698,  2699,  2700,  2701,  2702,  2703,  2704,  2705,  2706,
    2707,  2708,  2709,  2710,  2711,  2712,  2713,  2714,  2715,  2718,
    2720,  2723,  2725,  2734,  2736,  2737,  2740,  2742,  2746,  2747,
    2750,  2760,  2781,  2783,  2787,  2787,  2790,  2792,  2795,  2797,
    2801,  2805,  2806,  2811,  2814,  2817,  2822,  2825,  2830,  2833,
    2837,  2841,  2844,  2845,  2846,  2847,  2848,  2849,  2850,  2851,
    2857,  2864,  2865,  2866,  2871,  2872,  2878,  2885,  2886,  2887,
    2888,  2889,  2890,  2891,  2892,  2893,  2894,  2895,  2896,  2897,
    2901,  2902,  2903,  2908,  2909,  2910,  2911,  2912,  2915,  2916,
    2917,  2918,  2919,  2920,  2921,  2924,  2926,  2929,  2931,  2935,
    2947,  2954,  2961,  2962,  2963,  2964,  2965,  2968,  2978,  2987,
    2991,  2997,  2998,  3002,  3007,  3013,  3016,  3021,  3022,  3023,
    3024,  3025,  3029,  3030,  3033,  3035,  3038,  3040,  3044,  3051,
    3051,  3051,  3051,  3051,  3051,  3051,  3051,  3051,  3051,  3051,
    3051,  3051,  3051,  3051,  3051,  3051,  3052,  3052,  3052,  3052,
    3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,
    3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,
    3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,
    3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,  3052,
    3052,  3052,  3052,  3053,  3053,  3053,  3053,  3053,  3053,  3053,
    3053,  3053,  3054,  3054,  3054,  3054,  3055,  3055,  3055,  3056,
    3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,
    3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,
    3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,  3056,  3057,
    3057,  3058,  3059,  3060,  3061,  3062,  3063,  3064,  3065,  3066,
    3067,  3068,  3068,  3068,  3068,  3068,  3068
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END", "CLI_MARKER", "INVALID_TOKEN",
  "ELSECOL", "DDOT", "GEQ", "LEQ", "NEQ", "AND", "OR", "PO", "PC", "NUM",
  "ENUM", "RTRID", "IPA", "SYM", "TEXT", "PREFIX_DUMMY", "'='", "'<'",
  "'>'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'.'", "DEFINE",
  "ON", "OFF", "YES", "NO", "S", "MS", "US", "PORT", "LOG", "SYSLOG",
  "ALL", "DEBUG", "TRACE", "INFO", "REMOTE", "WARNING", "ERROR", "AUTH",
  "FATAL", "BUG", "STDERR", "SOFT", "TIMEFORMAT", "ISO", "OLD", "SHORT",
  "LONG", "BASE", "NAME", "CONFIRM", "UNDO", "CHECK", "TIMEOUT", "LATENCY",
  "LIMIT", "WATCHDOG", "CONFIGURE", "DOWN", "KERNEL", "PERSIST", "SCAN",
  "TIME", "LEARN", "DEVICE", "ROUTES", "GRACEFUL", "RESTART", "KRT_SOURCE",
  "KRT_METRIC", "MERGE", "PATHS", "TABLE", "KRT_PREFSRC", "KRT_REALM",
  "KRT_MTU", "KRT_WINDOW", "KRT_RTT", "KRT_RTTVAR", "KRT_SSTRESH",
  "KRT_CWND", "KRT_ADVMSS", "KRT_REORDERING", "KRT_HOPLIMIT",
  "KRT_INITCWND", "KRT_RTO_MIN", "KRT_INITRWND", "KRT_QUICKACK",
  "KRT_LOCK_MTU", "KRT_LOCK_WINDOW", "KRT_LOCK_RTT", "KRT_LOCK_RTTVAR",
  "KRT_LOCK_SSTRESH", "KRT_LOCK_CWND", "KRT_LOCK_ADVMSS",
  "KRT_LOCK_REORDERING", "KRT_LOCK_HOPLIMIT", "KRT_LOCK_RTO_MIN",
  "KRT_FEATURE_ECN", "KRT_FEATURE_ALLFRAG", "ROUTER", "ID", "PROTOCOL",
  "TEMPLATE", "PREFERENCE", "DISABLED", "DIRECT", "INTERFACE", "IMPORT",
  "EXPORT", "FILTER", "NONE", "STATES", "FILTERS", "RECEIVE", "ACTION",
  "WARN", "BLOCK", "DISABLE", "KEEP", "FILTERED", "PASSWORD", "FROM",
  "PASSIVE", "TO", "EVENTS", "PACKETS", "PROTOCOLS", "INTERFACES",
  "PRIMARY", "STATS", "COUNT", "FOR", "COMMANDS", "PREEXPORT", "NOEXPORT",
  "GENERATE", "ROA", "LISTEN", "BGP", "V6ONLY", "DUAL", "ADDRESS",
  "PASSWORDS", "DESCRIPTION", "SORTED", "RELOAD", "IN", "OUT", "MRTDUMP",
  "MESSAGES", "RESTRICT", "MEMORY", "IGP_METRIC", "CLASS", "DSCP", "WAIT",
  "MAX", "FLUSH", "AS", "SHOW", "STATUS", "SUMMARY", "ROUTE", "SYMBOLS",
  "ADD", "DELETE", "DUMP", "RESOURCES", "SOCKETS", "NEIGHBORS",
  "ATTRIBUTES", "EVAL", "ECHO", "ENABLE", "FUNCTION", "PRINT", "PRINTN",
  "UNSET", "RETURN", "ACCEPT", "REJECT", "QUITBIRD", "INT", "BOOL", "IP",
  "PREFIX", "PAIR", "QUAD", "EC", "SET", "STRING", "BGPMASK", "BGPPATH",
  "CLIST", "ECLIST", "IF", "THEN", "ELSE", "CASE", "TRUE", "FALSE", "RT",
  "RO", "UNKNOWN", "GENERIC", "GW", "NET", "MASK", "PROTO", "SOURCE",
  "SCOPE", "CAST", "DEST", "IFNAME", "IFINDEX", "LEN", "DEFINED",
  "CONTAINS", "RESET", "PREPEND", "FIRST", "LAST", "MATCH", "ROA_CHECK",
  "EMPTY", "WHERE", "ROLE_MAP", "NET_PREFIX", "PREFIX_ROLE", "BFD", "MIN",
  "IDLE", "RX", "TX", "INTERVAL", "MULTIPLIER", "MULTIHOP", "NEIGHBOR",
  "DEV", "LOCAL", "SESSIONS", "HOLD", "CONNECT", "RETRY", "KEEPALIVE",
  "STARTUP", "VIA", "NEXT", "HOP", "SELF", "DEFAULT", "PATH", "METRIC",
  "START", "DELAY", "FORGET", "AFTER", "BGP_PATH", "BGP_LOCAL_PREF",
  "BGP_MED", "BGP_ORIGIN", "BGP_NEXT_HOP", "BGP_ATOMIC_AGGR",
  "BGP_AGGREGATOR", "BGP_COMMUNITY", "BGP_EXT_COMMUNITY", "RR", "RS",
  "CLIENT", "CLUSTER", "AS4", "ADVERTISE", "IPV4", "CAPABILITIES",
  "PREFER", "OLDER", "MISSING", "LLADDR", "DROP", "IGNORE", "REFRESH",
  "INTERPRET", "COMMUNITIES", "BGP_ORIGINATOR_ID", "BGP_CLUSTER_LIST",
  "IGP", "GATEWAY", "RECURSIVE", "MED", "TTL", "SECURITY", "DETERMINISTIC",
  "SECONDARY", "ALLOW", "AWARE", "LINK", "EXTENDED", "ROLE", "PEER",
  "PROVIDER", "CUSTOMER", "INTERNAL", "COMPLEX", "STRICT_MODE", "USE",
  "OSPF", "AREA", "OSPF_METRIC1", "OSPF_METRIC2", "OSPF_TAG",
  "OSPF_ROUTER_ID", "RFC1583COMPAT", "STUB", "TICK", "COST", "COST2",
  "RETRANSMIT", "HELLO", "TRANSMIT", "PRIORITY", "DEAD", "TYPE",
  "BROADCAST", "BCAST", "NONBROADCAST", "NBMA", "POINTOPOINT", "PTP",
  "POINTOMULTIPOINT", "PTMP", "SIMPLE", "AUTHENTICATION", "STRICT",
  "CRYPTOGRAPHIC", "ELIGIBLE", "POLL", "NETWORKS", "HIDDEN", "VIRTUAL",
  "ONLY", "BUFFER", "LARGE", "NORMAL", "STUBNET", "TAG", "EXTERNAL",
  "LSADB", "ECMP", "WEIGHT", "NSSA", "TRANSLATOR", "STABILITY", "GLOBAL",
  "LSID", "INSTANCE", "REAL", "NETMASK", "LENGTH", "LSA", "SUPPRESSION",
  "TOPOLOGY", "STATE", "PIPE", "MODE", "OPAQUE", "TRANSPARENT", "RIP",
  "INFINITY", "PERIOD", "GARBAGE", "MULTICAST", "QUIET", "NOLISTEN",
  "VERSION1", "PLAINTEXT", "MD5", "HONOR", "NEVER", "ALWAYS", "RIP_METRIC",
  "RIP_TAG", "STATIC", "PROHIBIT", "MULTIPATH", "BLACKHOLE", "UNREACHABLE",
  "';'", "'('", "')'", "':'", "'{'", "'}'", "','", "'?'", "'['", "']'",
  "$accept", "config", "conf_entries", "definition", "expr", "expr_us",
  "bool", "ipa", "ipa_scope", "prefix", "prefix_or_ipa", "pxlen",
  "datetime", "text", "text_or_none", "log_config", "syslog_name",
  "log_file", "log_mask", "log_mask_list", "log_cat", "mrtdump_base",
  "timeformat_which", "timeformat_spec", "timeformat_base", "debug_unix",
  "cmd_CONFIGURE", "cmd_CONFIGURE_SOFT", "cmd_CONFIGURE_CONFIRM",
  "cmd_CONFIGURE_UNDO", "cmd_CONFIGURE_CHECK", "cmd_DOWN", "cfg_name",
  "cfg_timeout", "kern_proto_start", "kern_item", "kif_proto_start",
  "kif_item", "kern_sys_item", "rtrid", "idval", "listen", "listen_opts",
  "listen_opt", "gr_opts", "tab_sorted", "newtab", "roa_table_start",
  "roa_table_opts", "roa_table", "proto_start", "proto_name", "proto_item",
  "imexport", "limit_action", "limit_spec", "rtable", "debug_default",
  "iface_patt_node_init", "iface_patt_node_body", "iface_negate",
  "iface_patt_node", "iface_patt_list", "iface_patt_list_nopx",
  "iface_patt_init", "iface_patt", "tos", "dev_proto_start", "dev_proto",
  "dev_iface_init", "dev_iface_patt", "debug_mask", "debug_list",
  "debug_flag", "mrtdump_mask", "mrtdump_list", "mrtdump_flag",
  "password_list", "password_items", "password_item",
  "password_item_begin", "password_item_params", "cmd_SHOW_STATUS",
  "cmd_SHOW_MEMORY", "cmd_SHOW_PROTOCOLS", "cmd_SHOW_PROTOCOLS_ALL",
  "optsym", "cmd_SHOW_INTERFACES", "cmd_SHOW_INTERFACES_SUMMARY",
  "cmd_SHOW_ROUTE", "r_args", "export_mode", "cmd_SHOW_ROA", "roa_args",
  "roa_mode", "cmd_SHOW_SYMBOLS", "sym_args", "roa_table_arg",
  "cmd_ADD_ROA", "cmd_DELETE_ROA", "cmd_FLUSH_ROA", "cmd_DUMP_RESOURCES",
  "cmd_DUMP_SOCKETS", "cmd_DUMP_EVENTS", "cmd_DUMP_INTERFACES",
  "cmd_DUMP_NEIGHBORS", "cmd_DUMP_ATTRIBUTES", "cmd_DUMP_ROUTES",
  "cmd_DUMP_PROTOCOLS", "cmd_EVAL", "cmd_ECHO", "echo_mask", "echo_size",
  "cmd_DISABLE", "cmd_ENABLE", "cmd_RESTART", "cmd_RELOAD",
  "cmd_RELOAD_IN", "cmd_RELOAD_OUT", "cmd_DEBUG", "cmd_MRTDUMP",
  "cmd_RESTRICT", "proto_patt", "proto_patt2", "filter_def", "$@1",
  "role_map_def", "$@2", "filter_eval", "type", "one_decl", "decls",
  "declsn", "filter_body", "role_name", "role_map_body", "filter",
  "role_map", "where_filter", "function_params", "function_body",
  "function_def", "$@3", "cmds", "cmds_int", "block", "fipa", "set_atom",
  "switch_atom", "pair_expr", "pair_atom", "pair_item", "ec_expr",
  "ec_kind", "ec_item", "set_item", "switch_item", "set_items",
  "switch_items", "fprefix_s", "fprefix", "fprefix_set", "switch_body",
  "bgp_path_expr", "bgp_path", "bgp_path_tail1", "bgp_path_tail2",
  "constant", "constructor", "rtadot", "function_call", "symbol",
  "static_attr", "term", "break_command", "print_one", "print_list",
  "var_listn", "var_list", "cmd", "bfd_proto_start", "bfd_proto_item",
  "bfd_proto_opts", "bfd_proto", "bfd_iface_start", "bfd_iface_item",
  "bfd_iface_opts", "bfd_iface_opt_list", "bfd_iface", "bfd_multihop",
  "bfd_neigh_iface", "bfd_neigh_local", "bfd_neigh_multihop",
  "bfd_neighbor", "cmd_SHOW_BFD_SESSIONS", "bgp_proto_start",
  "bgp_nbr_opts", "bgp_proto", "ospf_proto_start", "ospf_proto",
  "ospf_proto_item", "ospf_area_start", "ospf_area", "ospf_area_opts",
  "ospf_area_item", "$@4", "$@5", "ospf_stubnet", "ospf_stubnet_start",
  "ospf_stubnet_opts", "ospf_stubnet_item", "ospf_vlink",
  "ospf_vlink_opts", "ospf_vlink_item", "ospf_vlink_start",
  "ospf_iface_item", "pref_list", "pref_item", "pref_base", "pref_opt",
  "nbma_list", "nbma_eligible", "nbma_item", "ospf_iface_start",
  "ospf_instance_id", "ospf_iface_patt_list", "$@6", "ospf_iface_opts",
  "ospf_iface_opt_list", "ospf_iface", "opttext", "cmd_SHOW_OSPF",
  "cmd_SHOW_OSPF_NEIGHBORS", "cmd_SHOW_OSPF_INTERFACE",
  "cmd_SHOW_OSPF_TOPOLOGY", "cmd_SHOW_OSPF_TOPOLOGY_ALL",
  "cmd_SHOW_OSPF_STATE", "cmd_SHOW_OSPF_STATE_ALL", "cmd_SHOW_OSPF_LSADB",
  "lsadb_args", "pipe_proto_start", "pipe_proto", "rip_cfg_start",
  "rip_cfg", "rip_auth", "rip_mode", "rip_iface_item", "rip_iface_opts",
  "rip_iface_opt_list", "rip_iface_init", "rip_iface",
  "static_proto_start", "static_proto", "stat_route0", "stat_multipath1",
  "stat_multipath", "stat_route", "stat_route_item", "stat_route_opts",
  "stat_route_opt_list", "cmd_SHOW_STATIC", "conf", "cli_cmd", "proto",
  "kern_proto", "kif_proto", "dynamic_attr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    61,    60,    62,   126,    43,    45,    42,    47,
      37,    33,    46,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,   610,   611,   612,   613,
     614,   615,   616,   617,   618,   619,   620,   621,   622,   623,
     624,   625,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
      59,    40,    41,    58,   123,   125,    44,    63,    91,    93
};
# endif

#define YYPACT_NINF -1409

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1409)))

#define YYTABLE_NINF -460

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-460)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     115,  2776,   251,   664,   728,   739,   164,   728,   728,   339,
     728,   198,   137,  1060,   221,   282,   998,  1354,   121,   728,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
     254,   707,   334,   429,   558,   305,   426,   384, -1409, -1409,
     524,   506,   459,   424,  1354,   604,   606, -1409, -1409, -1409,
   -1409, -1409, -1409,   119,   119, -1409, -1409, -1409, -1409,   245,
   -1409,  1047, -1409,   119,  2340, -1409, -1409, -1409, -1409,   119,
   -1409,   119,  2506,   119,  2200,   119,  2269,   119,  2154,   119,
    2104, -1409, -1409,  2287,  2456, -1409, -1409, -1409,   183, -1409,
     650,   658,   674,   650,   618, -1409,   696,   698,   728,   728,
     714,   204, -1409,   634,   781,   123, -1409,   731,   741, -1409,
   -1409,   492,   462,   735,   559,   559,   760,   765,   771,   773,
     783,   785,   789,   792,    27, -1409, -1409, -1409,   778,   412,
   -1409,   578,    89,    15,  1354, -1409,   431,   439,   449, -1409,
   -1409,   452,   456,   458, -1409,   913,    94, -1409, -1409, -1409,
   -1409, -1409,  2028, -1409,  2956, -1409, -1409,  1565,   848,   862,
     847, -1409, -1409,   810, -1409, -1409,    25,   209,   183,   237,
   -1409, -1409, -1409, -1409,   391,   478,   237,   237,   715,   747,
      87, -1409,   893, -1409,   204,   518,  2981, -1409, -1409,   799,
     908,   537,   539, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409,   541,   183,   965,   872,   237,   457, -1409,
     550,   738,   920,   791,   204, -1409,   593,   595,   598,   601,
      10,   704,   947,   629,  1020,   774,   791,   457,  1002,   982,
     355,   916,   457,   140,   559,   174,   999,    46,  1003,   825,
     821,    80,   820,   817,   538,   808,   803,   457,   804,   805,
     798,    -1,   -20,   829,   793,   797,   457,   849,   583,   705,
   -1409,   706,   703,   749,    26,   457,   996,   237,   457,  1004,
   -1409, -1409,   710,   708, -1409,   712,  1035,   445, -1409,   721,
     718,   237,  1050, -1409,   791,   729,   -71,   237,   237,  1062,
     -61, -1409,   740,   742, -1409,   744,   748,   853,   559,  1059,
   -1409,   756,   504,   759,  1079,   457,  1090,   457,  1105,  1107,
    1101, -1409,   788,   790,   796,  1116,  1172, -1409,   802,   806,
   -1409, -1409,   900,  1195,   618, -1409, -1409,  1200,   237,  1202,
   -1409, -1409,  1204,  1209, -1409, -1409, -1409,    62,  1213,  1198,
    1216, -1409, -1409,   815,  1217, -1409,  1218,   281, -1409, -1409,
    2707,   668,   735, -1409,   735,   735, -1409,   359,   435,  1223,
    1224, -1409, -1409,    -4,  1058,  1065, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409,    27, -1409,    27,  1354,    27,    27,
    1215, -1409,  1226,   956,  1212,   992,  1225,    15,    15,  1214,
    1221,  1354,  1354,  1354,  1354,  1354,  1235, -1409, -1409,  1230,
   -1409,   850,    53, -1409, -1409, -1409, -1409,   676, -1409,  1248,
   -1409, -1409, -1409,   266,   176, -1409,   279, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,  1354,  1354,
    1354,  1354,  1354,  1354,  1354,  1354,  1354,  1354,  1354,  1354,
    1354,   690, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409,   546, -1409, -1409,  1256, -1409,  1354,   791, -1409, -1409,
    1565,   861, -1409, -1409, -1409, -1409, -1409, -1409,   237,  1354,
   -1409, -1409, -1409, -1409,   237,   895,   964, -1409,   672, -1409,
   -1409,   237, -1409, -1409, -1409, -1409, -1409, -1409, -1409,  1354,
     863,  2854, -1409,   201,   864, -1409,   866,  2854,  1246, -1409,
   -1409, -1409,   -80, -1409, -1409, -1409, -1409,    26, -1409, -1409,
   -1409, -1409,   145,  2690, -1409,  1135,  1354, -1409, -1409,   145,
   -1409,   145, -1409, -1409, -1409, -1409, -1409, -1409,  1194,  1197,
     457,    88, -1409,   873,  1227,   874,   878,   237,   135,   986,
     457,  1118,   559,   882, -1409,   883,  1255,   132,   237,  1115,
     237,  1229,  1232,   237,  1238,   105,   237,   237,   457,  1240,
     888,  1177,   892,   457,   899,   457,   423,   457,   965,   457,
     901,   905,   457,   457,   457,   906,  1121, -1409, -1409, -1409,
   -1409,   984,   930, -1409, -1409, -1409,   457, -1409, -1409,   457,
   -1409,  1252,   237, -1409, -1409, -1409,  1281,   931,   933, -1409,
   -1409,   934,   237, -1409,   935, -1409,  1182, -1409, -1409, -1409,
     936,   937,   939,   237,   943,   945,   946, -1409, -1409,   373,
   -1409,   457, -1409,   965, -1409, -1409,   902, -1409,   559, -1409,
    1078, -1409, -1409, -1409,   950,   237, -1409,   237, -1409,   457,
     457,   457, -1409, -1409, -1409,   237, -1409,   559, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409,   610, -1409, -1409,  1344,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409,   637, -1409, -1409,
   -1409, -1409,  1346, -1409, -1409, -1409,  1340, -1409, -1409,  1345,
     559, -1409, -1409,  1343,  1356, -1409,  2690, -1409, -1409, -1409,
   -1409,   559, -1409, -1409, -1409,  1365, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409,  1360,  1366,  1366,   904,
     735,  1366,   735,  1366, -1409, -1409,   237,   559, -1409,  1350,
    1372, -1409, -1409,   938, -1409, -1409, -1409, -1409,   120, -1409,
     993, -1409,  1361, -1409, -1409, -1409, -1409,   187,   284,   312,
    1023,   365,   -73, -1409,  1354, -1409,  1354, -1409,  1390,  1005,
    1007,  1300,   118,   167, -1409, -1409, -1409,  1383,  1384, -1409,
    2992,  2992,  2992,  3017,  3017,  2992,  2992,  2992,  2992,   614,
     614,  1221,  1221, -1409,  1014, -1409, -1409, -1409, -1409,  1565,
   -1409,   713, -1409,   651, -1409, -1409,  1470,  1385, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409,  1018, -1409,  1021,  1498, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409,    79,  1022,  1000, -1409, -1409,   237, -1409, -1409,
     559, -1409, -1409, -1409,  1355,  2854, -1409, -1409, -1409,   559,
   -1409, -1409,  1397, -1409,  1025, -1409,  1298, -1409, -1409, -1409,
   -1409,   457,  2981, -1409, -1409,   753,   237,   237,  1029,   237,
    1032,  1033, -1409,   457, -1409, -1409,  1043,  1046,  1051,  1053,
     457,  1054,   457,  1056, -1409, -1409,  1438, -1409,   237,   237,
   -1409,  1063,   237,  1066,   237,   237,  1067,   237,  1069,  1070,
    1073,  1074,  1075,   237, -1409,    26, -1409,  1076, -1409,  1083,
    1085,  1086,  1087,  1089,  1091,  1100, -1409, -1409,  1103,  1104,
    1111, -1409,   202,  2723, -1409, -1409, -1409,   237, -1409,  2236,
    1112, -1409, -1409, -1409,  1113,  -105, -1409,  1055,  1114, -1409,
   -1409, -1409,  1119, -1409, -1409, -1409,   237,   315,   349,  1057,
    1128,  1129, -1409,  1255, -1409,   559,   -36,  1078,  1964, -1409,
   -1409, -1409, -1409, -1409,  1433, -1409,    -4, -1409, -1409, -1409,
     900, -1409, -1409,    62, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409,  1455,  1462, -1409, -1409,    26,
   -1409, -1409,    26,  1516, -1409,    26,  1366,  1531,  1366,  1532,
   -1409, -1409,  1364,  1367, -1409,  1354, -1409,  1510,  1354,  1354,
    1354, -1409,  1354, -1409,  1354,   547,  1597,  1354,   981,  1354,
   -1409, -1409,  1354, -1409, -1409,  1134,   778, -1409,  1354, -1409,
   -1409, -1409, -1409, -1409, -1409,  1025, -1409, -1409, -1409, -1409,
    2854,  1530, -1409, -1409, -1409,  1141,  1143, -1409,  1373, -1409,
     961, -1409,   333, -1409, -1409, -1409, -1409,   559, -1409, -1409,
    1146,  1142,  1147, -1409,  1158, -1409, -1409,  1164, -1409, -1409,
   -1409, -1409,  1165, -1409,  1166, -1409, -1409,   235, -1409, -1409,
   -1409,  1170, -1409,  1171,  1174, -1409,  1175, -1409, -1409, -1409,
   -1409, -1409,  1178,  1179, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409,  1181, -1409, -1409,  1183,
   -1409, -1409,   457,   256,   313, -1409,  1263,   559, -1409, -1409,
     432, -1409,  1185, -1409,  1220, -1409, -1409, -1409, -1409, -1409,
    1182, -1409,  1186,  1557,  1557,  1557,  1557, -1409, -1409, -1409,
   -1409,  1255,   457,   237,   -36,     9, -1409,  1560, -1409, -1409,
    1188,  1354, -1409, -1409, -1409,  1354,  1354,   677,   457, -1409,
    2028,  1190,  1231, -1409, -1409,   237, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409,  1589, -1409,  1590, -1409,  1579,  1581,
   -1409, -1409,  1655,  1681,  1706,  1741,   605,  1354, -1409, -1409,
    2981,  1196,  1191,  2981,  1300,  1586,  1766, -1409,  1222,    41,
   -1409,  2854, -1409,  1587,   559, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409,  1199, -1409,    38, -1409, -1409,   237,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409,   237,   237,   457,
     237, -1409,  1233,    26, -1409, -1409,  1234,  1236,   237, -1409,
   -1409,   237,  1237,   509, -1409,   373, -1409,  1228,  1242,  1243,
    1244, -1409, -1409, -1409,  1354,   956,  1354, -1409,  1968,  2708,
     819,  1245,  1249,  1608,   505, -1409,  2981,  1247,  1250, -1409,
   -1409, -1409,   634,   634, -1409, -1409, -1409, -1409,  1354,  1791,
    1629,  1421,  1241, -1409, -1409, -1409, -1409,  1476, -1409,  1025,
    1199, -1409, -1409,  1633,   791,  1402,  1257,  1025,  1264, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
     293,   237,  1352,   551, -1409,  1259, -1409,   373,   373,   373,
     373,  1993,  1258,  2018,  2383, -1409,  1988, -1409, -1409, -1409,
    1354,  1354,   475,  1231, -1409,  1658,  1666,  1816, -1409,  1269,
    1270,    22, -1409,  1656, -1409,   403, -1409, -1409,   559,  1423,
   -1409,  1220, -1409, -1409,    14,    33,    30,  1879,   237,   237,
     237, -1409, -1409,   515, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,  1272,  1530,
    1464, -1409,    20,  2043,  2068,  1275,  1284,  1285,  1287,  1294,
   -1409, -1409, -1409, -1409,  1354, -1409,  1354, -1409,  1295,   457,
     807,  1449,   237,   237, -1409,  1297, -1409,   457, -1409, -1409,
    2532, -1409, -1409, -1409,    47,   457,   237,   457, -1409,  1299,
   -1409,   237,   237,   237,  1430,    82,   -85, -1409, -1409,  1301,
   -1409, -1409, -1409,  1347, -1409,  1311,  1307,  1988,    77, -1409,
   -1409, -1409,   676, -1409, -1409,  1719, -1409, -1409, -1409,   325,
   -1409, -1409,  1354,  1354,  1354,  1354, -1409,  1321,  1334, -1409,
   -1409,  1489,  1492,  1493, -1409, -1409, -1409, -1409,  1434,   237,
    1341,   457,  1392,   212,  1440,   457,  1375,   457,   237,   237,
     237,  1480,   237,   147,   835,  1386,    16,  1416,   237,  1396,
    1424, -1409, -1409,  1359, -1409,   237,  1371, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409,   237,   237, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409,  1841,    56,    77,    72,
    1866,  1893,  1918,  1943,  1354, -1409,   237,   237,   237,   457,
   -1409, -1409, -1409,   159,   237,   237, -1409,   621, -1409,  1388,
   -1409, -1409, -1409, -1409,   237, -1409,   237, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409,   457, -1409, -1409, -1409,
     457, -1409,   237,   457, -1409, -1409, -1409, -1409, -1409, -1409,
    1354, -1409, -1409, -1409,  1379,  1380,  1381,  1382,  1394, -1409,
   -1409, -1409, -1409,    32, -1409, -1409, -1409, -1409, -1409,  1431,
   -1409,   457, -1409, -1409, -1409, -1409, -1409, -1409,  1841, -1409,
   -1409, -1409, -1409, -1409, -1409,  1435, -1409, -1409, -1409, -1409,
    1387, -1409
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   396,     0,     0,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,   817,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,   828,   829,   830,   831,   832,   833,   834,   835,
     836,   837,   838,   839,   840,   841,   842,     3,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
       0,     0,     0,     0,   396,     0,     0,   779,   780,   781,
     782,   783,   784,   117,   117,   785,   786,   787,   788,   113,
     789,     0,   791,   117,   121,   792,   793,   794,   795,   117,
     846,   117,   121,   117,   121,   117,   121,   117,   121,   117,
     121,     5,   790,   121,   121,   272,   274,   273,     0,    76,
      75,     0,     0,    75,    77,    74,     0,     0,     0,     0,
       0,     0,   271,   243,   276,     0,   226,     0,     0,   209,
     234,     0,   205,   205,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   383,   392,   389,   323,   398,
     386,     0,     0,   382,   396,   428,     0,     0,     0,   384,
     385,     0,     0,     0,   430,   396,     0,   387,   388,   393,
     426,   427,     0,   425,     0,   259,   258,     0,   261,     0,
       0,    30,    29,    35,    38,    36,     0,    18,     0,     0,
      57,    56,    55,    54,     0,     0,     0,     0,     0,   107,
       0,   278,     0,   100,     0,     0,   282,   315,   280,   118,
       0,     0,     0,   111,    80,    88,   163,   508,   478,   574,
     710,   716,   751,     0,     0,     0,     0,     0,    18,   167,
       0,     0,     0,     0,     0,   845,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,     0,    18,     0,   509,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
     847,     0,     0,     0,     0,    18,     0,     0,    18,     0,
     848,   577,     0,     0,   585,     0,     0,     0,   849,     0,
       0,     0,     0,   749,     0,     0,     0,     0,     0,     0,
       0,   850,     0,     0,   188,   192,     0,     0,     0,     0,
     851,     0,     0,   776,     0,    18,     0,    18,     0,     0,
       0,   843,     0,     0,     0,     0,    32,   844,     0,     0,
     170,   169,     0,     0,    77,    71,    72,     0,    78,     0,
     265,   263,     0,     0,   266,   181,   180,     0,     0,     0,
       0,   275,   277,   276,     0,   206,     0,   230,   201,   200,
       0,     0,   205,   204,   205,   205,   701,   205,   205,     0,
       0,    19,    20,     0,     0,     0,   254,   250,   255,   251,
     248,   249,   252,   253,   379,   398,   379,   396,   379,   379,
       0,   371,     0,   396,     0,     0,     0,   382,   382,     0,
     423,   396,   396,   396,   396,   396,     0,   342,   343,     0,
     345,     0,     0,   324,   327,   325,   329,   396,   326,   351,
     349,   350,   357,     0,   362,   366,     0,   889,   859,   860,
     861,   862,   863,   864,   865,   866,   867,   868,   869,   870,
     871,   872,   873,   874,   875,   876,   877,   878,   879,   880,
     881,   882,   883,   884,   885,   886,   887,   399,   888,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   891,   894,
     893,   890,   892,   895,   896,   897,   900,   898,   899,   901,
     902,   903,   904,   905,   906,   429,   431,   256,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,     0,    43,    44,    45,    46,    47,    48,    49,    50,
      51,     0,    41,   262,     0,   264,   396,     0,    37,    39,
       0,     0,     7,     9,    14,    16,    15,    17,     0,   396,
      13,    65,   147,   148,    58,     0,     0,    64,     0,    67,
      68,     0,   108,   109,    94,    96,    97,    98,   159,   396,
       0,   297,   110,     0,     0,    53,     0,   297,     0,   119,
     852,   856,     0,   164,   124,   146,   132,     0,   122,   123,
     149,   137,     0,   297,   138,     0,   396,   126,   136,     0,
     127,     0,   134,   125,   165,   166,   483,   512,     0,     0,
      18,    18,   524,     0,     0,     0,     0,     0,    18,     0,
      18,     0,     0,     0,   525,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
       0,     0,     0,    18,     0,    18,     0,    18,     0,    18,
       0,     0,    18,    18,    18,     0,     0,   302,   303,   304,
     305,     0,     0,   573,   513,   575,    18,   586,   578,    18,
     583,   580,     0,   576,   588,   711,     0,     0,     0,   712,
     717,     0,     0,   149,     0,   193,   189,   732,   730,   731,
       0,     0,     0,     0,     0,     0,     0,   718,   725,   194,
     752,    18,   757,     0,   753,   768,     0,   767,     0,   771,
       0,   769,   770,   774,     0,     0,    81,     0,    83,    18,
      18,    18,   854,   855,   853,     0,    31,     0,   858,   857,
     175,   174,   176,   178,   179,   177,     0,   172,   269,     0,
      73,    79,    69,   267,   268,   185,   186,     0,   183,   270,
     244,   247,     0,   202,   207,   225,     0,   232,   231,     0,
       0,   208,   215,     0,     0,   223,   297,   217,   216,   220,
     221,     0,   222,   224,   210,     0,   214,   233,   242,   235,
     239,   240,   237,   241,   236,   238,     0,   692,   692,     0,
     205,   692,   205,   692,   693,   778,     0,     0,    23,     0,
       0,   375,   376,     0,   377,   378,   373,   361,   457,   460,
       0,   437,     0,   438,   380,   381,   374,     0,     0,     0,
       0,     0,     0,   344,   396,   409,   396,   338,   336,     0,
       0,   335,     0,     0,   390,   363,   364,     0,     0,   391,
     421,   419,   417,   414,   415,   416,   418,   420,   422,   410,
     411,   412,   413,   432,     0,   433,   435,   436,   260,     0,
     257,     0,    34,     0,    33,    66,     0,     0,    60,    61,
      62,    63,    10,    11,    12,     0,   149,     0,     0,    92,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,     0,     0,     0,   279,   301,     0,   104,   105,
       0,    99,   101,    52,     0,   297,   281,   306,   120,     0,
     114,   133,   153,   156,   168,   145,   139,   129,   307,   308,
     135,    18,   311,   130,   128,   121,     0,     0,     0,     0,
       0,     0,   518,    18,   554,   556,     0,     0,     0,     0,
      18,     0,    18,     0,   570,   526,     0,   509,     0,     0,
     516,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,     0,   521,     0,   552,     0,
       0,     0,     0,     0,     0,     0,   532,   533,     0,     0,
       0,   558,     0,   297,   571,   582,   579,     0,   584,     0,
       0,   714,   715,   720,     0,   747,   729,     0,     0,   724,
     719,   721,     0,   727,   728,   726,     0,     0,     0,     0,
       0,     0,   764,    21,   766,     0,   761,   765,   396,   756,
      91,    82,    84,    85,    86,    89,    25,    24,    90,   171,
       0,    70,   182,     0,   203,   229,   228,   227,   212,   219,
     213,   211,   218,   507,   691,     0,     0,   700,   709,     0,
     707,   704,     0,     0,   702,     0,   692,     0,   692,     0,
      26,    27,     0,     0,   372,   396,   446,     0,   396,   396,
     396,   424,   396,   444,   396,     0,     0,   396,   396,   396,
     328,   323,   396,   352,   358,     0,     0,   367,   396,    42,
       6,    40,     8,    59,   106,   160,    93,    95,   296,   295,
     297,   396,   103,   102,   313,   299,     0,   316,     0,   154,
       0,   149,     0,   144,   131,   486,   486,     0,   485,   479,
       0,     0,     0,   569,     0,   565,   564,     0,   555,   559,
     560,   561,     0,   550,     0,   541,    22,     0,   510,   511,
     514,     0,   522,     0,     0,   545,     0,   528,   527,   540,
     539,   534,     0,     0,   553,   537,   529,   530,   531,   557,
     567,   536,   535,   568,   538,   562,     0,   309,   310,     0,
     581,   681,    18,     0,    18,   599,     0,     0,   601,   591,
      18,   587,     0,   605,   682,   713,   723,   745,   750,   187,
     189,   722,     0,     0,     0,     0,     0,   191,   754,   755,
     763,    21,    18,     0,   762,     0,   450,     0,   451,   452,
       0,   396,   448,   449,   447,   396,   396,     0,    18,   777,
       0,     0,   396,   772,   775,     0,   173,   184,   695,   694,
     708,   703,   705,   706,     0,   696,     0,   698,     0,     0,
     458,   439,     0,     0,     0,     0,     0,   396,   394,   337,
     335,     0,     0,   341,     0,     0,     0,   298,     0,   396,
     319,     0,   312,     0,   150,   151,   155,   157,   142,   140,
     141,   143,   149,   480,   495,   481,   499,   482,   484,     0,
     546,   566,   548,   549,   551,   517,   515,   544,   543,   523,
     542,   519,   563,   572,   149,   604,   592,     0,     0,    18,
       0,   590,     0,     0,   608,   603,   607,     0,     0,   597,
     589,     0,   615,   738,   190,   194,    28,     0,     0,     0,
       0,   758,   760,   759,   396,   396,   396,   396,     0,     0,
       0,     0,     0,     0,     0,   471,   453,   455,     0,    87,
     697,   699,   243,   243,   443,   441,   442,   440,   396,     0,
     339,   396,     0,   434,   314,   320,   300,     0,   152,   158,
     495,   493,   498,     0,     0,   502,     0,   684,   688,   594,
     595,   593,   596,   669,   629,   609,   669,   598,   683,   616,
       0,     0,     0,     0,   748,     0,   199,   194,   194,   194,
     194,     0,     0,     0,     0,   465,   396,   368,   464,   773,
     396,   396,     0,   396,   470,     0,     0,     0,   395,     0,
       0,     0,   365,     0,   497,     0,   500,   501,     0,   504,
     547,   682,   686,   690,     0,     0,     0,   618,     0,     0,
       0,   741,   739,    18,   733,   734,   735,   736,   737,   740,
     746,   195,   196,   197,   198,   463,   397,   468,     0,   396,
     461,   321,     0,     0,     0,     0,     0,     0,     0,     0,
     456,   245,   246,   445,   396,   348,   396,   346,     0,    18,
       0,     0,     0,     0,   496,     0,   503,    18,   506,   685,
       0,   600,   672,   670,   673,    18,     0,    18,   606,     0,
     602,     0,     0,     0,     0,     0,     0,   614,   628,     0,
     161,   162,   742,     0,   743,     0,     0,   396,   396,   330,
     334,   332,   396,   472,   333,   355,   353,   354,   359,     0,
     467,   466,   396,   396,   396,   396,   473,     0,     0,   112,
     492,     0,     0,     0,   487,   491,   494,   505,     0,     0,
       0,    18,     0,     0,     0,    18,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   689,   668,     0,   674,     0,     0,   612,   613,   611,
     610,   622,   620,   619,     0,     0,   623,   625,   626,   627,
     617,   744,   469,   322,   462,   370,   335,     0,   396,     0,
       0,     0,     0,     0,   396,   347,     0,     0,     0,    18,
     634,   676,   666,     0,     0,     0,   661,    18,   667,     0,
     650,   630,   633,   631,     0,   648,     0,   635,   637,   638,
     639,   640,   641,   642,   643,   644,    18,   655,   656,   657,
      18,   632,     0,    18,   687,   675,   671,   621,   624,   331,
     396,   356,   369,   360,     0,     0,     0,     0,     0,   488,
     489,   490,   651,     0,   659,   658,   660,   662,   663,     0,
     664,    18,   647,   636,   646,   649,   652,   645,     0,   477,
     475,   476,   474,   340,   654,   678,   677,   665,   653,   679,
       0,   680
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1409, -1409, -1409, -1409,  -207,  -216,   -67,  -260,  -937,  -150,
    -758, -1409,  -186,   -77, -1409, -1409, -1409, -1409, -1409,  1254,
     917, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409,   648,  1441, -1409, -1409, -1409, -1409, -1409, -1409,
    -292, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409,  1038,   -96,  1534, -1409,   -76,   -37, -1409, -1409, -1409,
   -1409,   685,  -682, -1409, -1409, -1409,   244, -1409, -1409, -1409,
   -1409,   -69, -1409,   767,    31, -1409,   766, -1284,   608,  -683,
   -1409,  -719, -1409, -1409, -1409, -1409,  -134, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409,  -293, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409,   949,  1427, -1409, -1409, -1409, -1409, -1409, -1409,
    -862,   701,   560,  1251,   585,   828,  1061, -1409,  1443, -1409,
    -570, -1409, -1409, -1341, -1409,   317,  -183,   988,   238, -1066,
     758, -1408, -1253,  1651, -1311,  1011,   248, -1409, -1409,  -179,
    1008, -1409, -1409, -1409, -1409,   463,   641, -1409, -1409,  -994,
   -1409,   -49,   625,   -17, -1409, -1409,   444,   784,   523, -1020,
   -1409, -1409, -1409, -1409,   734, -1409, -1409,   491, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409,   914, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409,   476, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409,   448, -1409, -1409, -1409, -1409, -1409,
    -501, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409,   854, -1409, -1409, -1409, -1409, -1409,
   -1409, -1409, -1409, -1409, -1409, -1409, -1089
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    88,   560,   569,   561,   403,   957,  1037,
    1038,   808,  1327,   205,   737,    89,   548,   206,   551,   541,
     542,    90,   214,   215,    91,    92,    20,    21,    22,    23,
      24,    25,   134,   369,    93,   352,    94,   358,   353,    95,
     580,    96,   583,   912,    97,   573,    98,    99,   592,   100,
     101,   231,   256,   607,  1123,   927,   596,   102,   922,  1276,
    1120,   923,   924,  1370,   886,   887,  1441,   103,   104,   600,
     257,   363,   746,   747,   378,   757,   758,   333,  1007,   334,
     335,  1019,    26,    27,    28,    29,   399,    30,    31,    32,
     390,   785,    33,   387,   770,    34,   391,   380,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,   198,   544,    48,    49,    50,    51,    52,    53,    54,
      55,    56,   128,   384,   105,   581,   106,   587,   107,   902,
     903,   904,  1116,   929,   672,   916,   930,  1179,   608,   915,
     906,   108,   586,  1268,  1269,  1460,   187,   449,  1525,   838,
     839,   450,  1262,   840,   451,   452,  1528,   453,  1529,   188,
     455,   456,  1462,   419,   189,   420,   429,   190,   191,   192,
    1231,   193,   515,  1260,  1232,  1347,  1348,   819,   820,  1270,
     109,  1130,   935,   110,  1282,  1485,  1425,  1372,  1283,  1285,
    1375,  1429,  1488,  1287,    57,   111,   637,   112,   113,   114,
     312,   313,   314,   999,  1192,  1312,  1317,  1315,  1316,  1435,
    1499,  1193,  1437,  1509,  1194,  1573,  1434,  1493,  1494,  1576,
    1663,  1690,  1686,  1304,  1322,  1378,  1431,  1490,  1433,  1305,
    1055,    58,    59,    60,    61,    62,    63,    64,    65,   799,
     115,   116,   117,   118,   700,  1449,  1395,  1323,  1198,   693,
     694,   119,   120,   342,  1026,  1027,   343,  1234,  1028,   724,
      66,   121,    67,   122,   123,   124,   516
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     194,   570,   563,   448,   404,   405,   225,   454,  1233,   568,
     568,  1005,   677,  1008,   636,   639,   301,   917,   311,   400,
     319,  1259,   332,  1051,   341,   806,  1518,   354,   359,  1476,
     427,  1334,   401,   402,  1230,  1519,  1520,  1521,  1091,  1587,
     598,   574,   414,   575,   576,   577,   415,  -318,   401,   402,
     401,   402,  1115,   697,  1526,   416,  -318,  -318,  -318,  -318,
    1215,   518,   519,   520,   521,   522,   635,   226,  1373,   549,
     919,  1519,  1520,  1521,  1091,   523,   524,   525,   526,   527,
     528,   529,   530,  -317,   658,   531,  1210,  1519,  1520,  1521,
    1091,  1216,  -317,  -317,  -317,  -317,  1215,   552,  1108,   660,
     680,   553,   574,   552,   575,   576,   577,   553,  1421,   443,
     444,   445,   168,   446,   691,   421,   425,  1230,  1516,     1,
     701,   702,   554,   555,   556,   557,   385,  1216,   518,   519,
     520,   521,   522,   443,   444,   445,  1091,   446,   229,   562,
    1637,  1344,   523,   524,   525,   526,   527,   528,   529,   530,
     552,  1527,   531,  1508,   553,   552,   195,   430,  1217,   553,
     552,   751,   552,   939,   553,   196,   553,   135,   442,   554,
     555,   556,   557,   958,   552,   594,   612,  1595,   553,   618,
     925,   599,   443,   444,   445,  1091,   446,   755,   712,   625,
     704,  1526,   401,   402,  1217,   518,   519,   520,   521,   522,
     626,   142,   845,   846,  1105,   633,  1572,  1212,  1495,   523,
     524,   525,   526,   527,   528,   529,   530,   552,   360,   531,
     654,   553,   578,  1538,   552,   756,  1585,   361,   553,   665,
    1218,  1219,  1220,  1221,  1222,  1223,  1224,   968,   678,   375,
     784,   681,   907,   554,   555,   556,   557,   695,   376,  1365,
    1225,    68,   552,  1226,   230,   584,   553,  1652,   796,  1588,
     797,   798,  1589,   801,   803,   659,  1218,  1219,  1220,  1221,
    1222,  1223,  1224,   200,  1331,  1230,   958,   558,   726,   619,
     728,   661,  1109,  1227,   765,   613,  1225,   143,  1527,  1226,
    1374,  1626,   518,   519,   520,   521,   522,  1056,   386,  1197,
    1067,  1121,  1069,   641,   959,   921,   523,   524,   525,   526,
     527,   528,   529,   530,   642,  1458,   531,   698,   699,  1227,
     518,   519,   520,   521,   522,   920,  1213,   426,   552,  1083,
     705,   706,   553,  1084,   523,   524,   525,   526,   527,   528,
     529,   530,   875,  1404,   531,  1117,   638,   554,   555,   556,
     557,   568,   646,   647,   908,   909,   910,   877,   125,   126,
    1638,  1496,  1275,  1639,   885,   421,   766,   421,   969,   421,
     421,   154,   953,   518,   519,   520,   521,   522,   393,  1438,
    1439,   947,   948,   127,  1497,   218,  1461,   523,   524,   525,
     526,   527,   528,   529,   530,   926,   940,   531,  1574,   807,
     813,   207,   926,   800,   926,  1575,   818,   959,  1537,  1115,
    1335,   564,  1230,  1278,   827,   828,   829,   830,   831,  1491,
     946,  1522,   428,   917,  1477,  1523,   767,   579,   417,   550,
     841,   961,   155,   963,   418,  1500,   966,  1684,  1498,   970,
     971,   768,  -318,   201,   202,   219,  -318,   552,   565,   566,
    1203,   553,  1204,   769,   393,   835,  1023,  1650,  1024,   836,
    1438,  1439,  1279,  1280,  1281,  1230,   554,   555,   556,   557,
     872,   210,   552,  1522,   208,   998,   553,  1036,  -317,   802,
     209,   393,  -317,   559,  1205,  1004,  1206,  1016,   579,   559,
     211,   554,   555,   556,   557,   447,  1012,  1461,   220,   138,
     139,   850,   851,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   862,  1664,  1665,  1368,  1008,  1030,  1092,
    1031,  1036,  1017,  1230,  1230,   197,  1075,  1411,  1035,   871,
     552,   629,   960,   933,   553,   934,   559,  1412,  1658,  1479,
     634,   559,   876,   221,   212,  1614,   559,  1071,   559,   554,
     555,   556,   557,   938,   941,   518,   519,   520,   521,   522,
     559,   949,   888,   951,   224,  1244,  1018,  1246,   447,   523,
     524,   525,   526,   527,   528,   529,   530,   401,   402,   531,
     847,   972,   394,  1615,  1307,  1308,   977,   362,   979,   932,
     983,   222,   985,  1078,   601,   988,   989,   990,  1465,  1070,
    1369,   911,  1175,   559,  1230,   213,  1396,   216,   377,   995,
     559,   223,   996,   518,   519,   520,   521,   522,   602,  1309,
    1047,   984,  1377,   227,   217,   228,  1440,   523,   524,   525,
     526,   527,   528,   529,   530,  1295,   552,   531,   559,   630,
     553,  1310,   529,   530,  1020,   395,   531,  1480,  1481,   233,
    1113,  1482,  1483,  1466,  1467,   554,   555,   556,   557,   448,
     448,   365,  1032,  1033,  1034,   631,  1066,    69,  1068,   454,
     129,   787,   843,   603,   604,   844,  1021,   366,  1451,  1452,
    1453,  1454,   605,  1163,   368,   848,   980,   788,   849,   164,
    1079,   165,   166,   167,   168,   169,   170,    70,   715,   370,
    1112,   371,   171,   172,   837,   173,    71,   174,  1468,    72,
     882,   883,   884,  1469,   559,   981,   982,   374,  1080,   379,
      73,   518,   519,   520,   521,   522,   201,   202,  1598,  1131,
    1132,  1599,  1134,    74,   388,   523,   524,   525,   526,   527,
     528,   529,   530,    75,   389,   531,   392,   125,   126,    76,
     203,  1148,  1149,   789,   393,  1151,  1390,  1153,  1154,   129,
    1156,   204,  1513,   406,   716,  1211,  1162,  1240,   407,  1118,
    1241,  1082,   127,  1243,   408,  1391,   409,    77,   364,    78,
      79,   367,   601,   790,   791,  1176,   410,    80,   411,   606,
    1180,   792,   412,   175,   130,   413,   717,  1318,   244,   176,
     381,   382,   131,   132,   133,   718,   609,   422,  1484,  1202,
     201,   202,  1392,   423,    81,    82,   424,  1085,   793,  1086,
     650,   651,   396,   687,   688,   383,    83,   518,   519,   520,
     521,   522,   431,   559,   381,   382,   397,   398,   245,  1129,
     432,   523,   524,   525,   526,   527,   528,   529,   530,    84,
     433,   531,    85,   434,   177,   178,   794,   435,   559,   436,
    1036,   603,   604,   543,  1124,   545,   246,  1286,  1669,   546,
     247,   248,   547,  1125,   250,   251,  1137,   811,   567,   812,
     252,   814,   815,  1142,   571,  1144,  1393,  1444,   863,   179,
     180,   437,   438,   439,   440,   667,   668,   669,   670,   671,
     719,   720,   721,   722,    86,   572,   181,  1057,   795,   182,
     253,   864,   582,   183,  1394,   254,   559,   184,   585,   865,
     401,   402,  1022,  1058,   866,   867,   164,   589,   165,   166,
     167,   168,   169,   170,   588,  1445,  1446,  1447,  1448,   171,
     172,   590,   173,   591,   174,   593,   518,   519,   520,   521,
     522,   868,   869,  1257,   878,   879,   136,   137,   140,   141,
     523,   524,   525,   526,   527,   528,   529,   530,   199,   164,
     531,   165,   166,   167,   168,   169,   170,   606,   740,   401,
     402,  1274,   171,   172,   595,   173,   597,   174,   611,   667,
     668,   669,   670,   614,   164,   615,   165,   166,   167,   168,
     169,   170,   616,  1126,  1127,   617,  1333,   171,   172,   837,
     173,  1358,   174,   620,  1036,  1039,  1040,  1059,  1328,  1329,
    1330,  1384,   559,   880,   881,   741,   742,   621,  1349,   622,
     175,   518,   519,   520,   521,   522,   176,  1314,   743,   744,
     623,   745,  1042,  1043,   624,   523,   524,   525,   526,   527,
     528,   529,   530,  1541,  1542,   531,  1101,   869,   818,  1415,
    1416,  1252,  1253,  1254,    87,  1255,   628,  1256,   824,   825,
     627,   632,  1263,   175,   640,  1264,   156,   185,   643,   176,
     644,  1266,  1376,   645,   186,   649,   648,   372,   373,   653,
     652,   177,   178,   655,   657,   662,   656,   663,   175,   664,
    1379,  1380,   666,  1382,   176,   673,   674,   675,   676,   679,
     683,  1387,   684,  1100,  1388,  1306,   685,  1311,   682,   234,
     686,   689,   690,  1319,   235,   692,   179,   180,   437,   438,
     439,   440,   232,   696,   177,   178,   157,   703,   158,   159,
     707,   243,   708,   181,   713,  1332,   182,   258,   709,   259,
     183,   302,   710,   315,   184,   320,   714,   336,  1128,   177,
     178,  1342,   711,   723,   725,   727,   236,  1060,  1486,   179,
     180,  1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,   160,
     161,   162,   163,   729,  1442,   731,   181,   730,   732,   182,
     733,   735,   736,   183,   179,   180,   734,   184,   748,   237,
     144,   145,   738,   750,  1338,   752,   739,   753,  1339,  1340,
     146,   181,   754,  1061,   182,  1346,   759,   760,   183,   761,
     763,   764,   184,  1407,  1062,   147,   804,   805,   809,   816,
     822,  1510,  1511,  1512,   148,   810,   149,   150,   821,  1063,
    1359,   817,  1381,   826,   164,   833,   165,   166,   167,   168,
     169,   170,   823,   531,   832,   842,   834,   171,   172,   870,
     173,   874,   174,   889,   913,   918,  1544,   914,   931,   936,
    1064,  1065,   937,   942,   944,   568,  1545,   943,   945,  1524,
     950,   952,   954,   955,  1492,   956,  1492,   962,   974,  1578,
     238,   975,   976,   992,  1581,  1582,  1583,  1427,  1586,   978,
    1000,   986,   993,   151,   964,   987,   991,   965,   518,   519,
     520,   521,   522,   967,   185,   973,   324,  1401,   818,  1403,
     997,   186,   523,   524,   525,   526,   527,   528,   529,   530,
     994,  1001,   531,  1002,  1003,  1006,  1009,  1010,  1025,  1011,
    1074,  1417,  1610,  1013,  1263,  1014,  1015,  1041,   175,  1044,
    1029,  1621,  1622,  1623,   176,  1625,  1627,   185,  -459,  1045,
    1046,  1641,  1048,  1053,   186,  1072,   239,   164,  1645,   165,
     166,   167,   168,   169,   170,  1049,  1514,  1647,  1648,   152,
     171,   172,   185,   173,  1052,   174,  1054,  1073,  1077,   186,
    1659,  1660,  1661,  1463,  1464,  1076,  1346,  1087,  1095,   568,
     568,   568,  1096,  1685,  1111,  1103,  1666,  1667,  1668,   177,
     178,  1088,  1540,  1089,  1524,  1098,  1524,  1672,  1104,  1673,
    1547,  1106,  1110,   240,  1119,  1081,  1122,   241,  1577,  1133,
    1579,  1121,  1135,  1136,   164,  1676,   165,   166,   167,   168,
     169,   170,   242,  1138,   179,   180,  1139,   171,   172,  1420,
     173,  1140,   174,  1141,  1143,   153,  1145,  1146,  1238,  1263,
    1199,   181,  1207,  1150,   182,  1239,  1152,  1155,   183,  1157,
    1158,   175,   184,  1159,  1160,  1161,  1164,   176,   518,   519,
     520,   521,   522,  1165,  1612,  1166,  1167,  1168,  1618,  1169,
    1620,  1170,   523,   524,   525,   526,   527,   528,   529,   530,
    1171,  1235,   531,  1172,  1173,  1596,   518,   519,   520,   521,
     522,  1174,  1195,  1196,  1200,  1600,  1601,  1602,  1603,  1201,
     523,   524,   525,   526,   527,   528,   529,   530,  1208,  1209,
     531,  1242,   177,   178,  1245,  1247,  1248,  1251,   175,  1249,
    1265,  1271,  1662,  1273,   176,  1272,  1288,  1290,  1289,  1215,
    1670,   890,   891,   892,   893,   894,   895,   896,  1291,   897,
     898,   899,   900,   901,  1292,  1293,  1294,   179,   180,  1674,
    1296,  1297,  1313,  1675,  1298,  1299,  1677,  1326,  1300,  1301,
    1216,  1302,  1336,  1303,   181,  1320,  1325,   182,  1321,  1337,
    1345,   183,  1350,  1351,  1352,   184,  1353,  1361,  1360,   177,
     178,  1362,  1367,  1371,  1688,   518,   519,   520,   521,   522,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   523,
     524,   525,   526,   527,   528,   529,   530,  1364,  1397,   531,
    1410,  -454,   185,  1678,   179,   180,  1419,  1383,  1385,   186,
    1386,  1389,  1398,  1399,  1400,  1408,  1422,  1217,  1423,  1409,
    1414,   181,  1426,  1413,   182,  1428,  1443,  1430,   183,  1450,
    1456,  1471,   184,   518,   519,   520,   521,   522,  1432,  1472,
    1474,  1478,  1475,  1487,  1515,  1517,  1532,   523,   524,   525,
     526,   527,   528,   529,   530,  1533,  1534,   531,  1535,   518,
     519,   520,   521,   522,  1536,  1539,  1543,  1546,  1584,  1580,
    1591,  1590,  1090,   523,   524,   525,   526,   527,   528,   529,
     530,  1592,  1593,   531,   518,   519,   520,   521,   522,  1218,
    1219,  1220,  1221,  1222,  1223,  1224,  1597,  1604,   523,   524,
     525,   526,   527,   528,   529,   530,  1605,  1606,   531,  1225,
    1607,  1608,  1226,  1609,  1617,  1611,  1613,  1619,  1624,   518,
     519,   520,   521,   522,  1640,   185,  1636,  1114,  1642,  1644,
    1643,  1671,   186,   523,   524,   525,   526,   527,   528,   529,
     530,  1646,  1227,   531,   518,   519,   520,   521,   522,  1679,
    1680,  1681,  1682,  1689,  1687,   610,  1099,  1691,   523,   524,
     525,   526,   527,   528,   529,   530,  1683,  1616,   531,   518,
     519,   520,   521,   522,   873,   749,  1277,  1236,  1324,  1237,
     762,  1267,  1341,   523,   524,   525,   526,   527,   528,   529,
     530,  1178,   185,   531,   518,   519,   520,   521,   522,   186,
    1093,  1366,   905,   786,  1594,  1651,   441,  1050,   523,   524,
     525,   526,   527,   528,   529,   530,  1261,  1653,   531,   518,
     519,   520,   521,   522,  1094,  1343,  1097,  1470,  1402,  1250,
    1284,  1424,  1436,   523,   524,   525,   526,   527,   528,   529,
     530,  1147,  1102,   531,   518,   519,   520,   521,   522,  1489,
       0,  1214,     0,     0,     0,     0,     0,     0,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,   531,     0,
    1107,   518,   519,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,   525,   526,   527,
     528,   529,   530,     0,     0,   531,   518,   519,   520,   521,
     522,     0,     0,     0,     0,  -317,     0,     0,     0,     0,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
     531,   518,   519,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,   525,   526,   527,
     528,   529,   530,     0,     0,   531,   518,   519,   520,   521,
     522,     0,     0,  1215,     0,     0,     0,     0,     0,     0,
     523,   524,   525,   526,   527,   528,   529,   530,     0,  1258,
     531,   518,   519,   520,   521,   522,     0,  1215,     0,     0,
       0,     0,     0,   324,  1216,   523,   524,   525,   526,   527,
     528,   529,   530,     0,     0,   531,   518,   519,   520,   521,
     522,     0,     0,   457,     0,   325,     0,     0,  1216,     0,
     523,   524,   525,   526,   527,   528,   529,   530,  1501,     0,
     531,   518,   519,   520,   521,   522,     0,  1354,     0,     0,
       0,     0,     0,     0,     0,   523,   524,   525,   526,   527,
     528,   529,   530,     0,     0,   531,   518,   519,   520,   521,
     522,  1217,     0,  1355,     0,     0,     0,     0,     0,     0,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
     531,     0,     0,     0,     0,  1217,     0,     0,  1356,   458,
     459,     0,     0,     0,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,     0,     0,  1357,     0,     0,     0,     0,     0,   244,
       0,     0,     0,  1218,  1219,  1220,  1221,  1222,  1223,  1224,
       0,     0,     0,   487,     0,     0,     0,     0,  1363,   337,
       0,     0,     0,  1225,     0,     0,  1226,  1218,  1219,  1220,
    1221,  1222,  1223,  1224,     0,     0,     0,     0,     0,   245,
       0,     0,     0,  1418,   488,   321,     0,  1225,     0,   244,
    1226,     0,     0,     0,     0,     0,  1227,  1228,     0,  1502,
    1503,  1504,     0,  1505,     0,     0,     0,   246,  1473,     0,
     322,   247,   248,     0,  1506,   250,   251,     0,     0,     0,
    1227,   252,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,  1649,     0,   244,     0,   489,   490,     0,
     491,   492,   493,   494,   495,   496,   497,     0,     0,     0,
       0,   253,     0,     0,     0,     0,   254,   246,  1654,     0,
       0,   247,   248,     0,   323,   250,   251,     0,     0,     0,
     338,   252,     0,   303,  1507,   245,     0,     0,   324,     0,
       0,     0,     0,     0,     0,  1655,     0,     0,     0,   498,
     499,   500,   501,   502,   503,   504,   505,   506,     0,     0,
     325,   253,     0,   246,   244,     0,   254,   247,   248,     0,
    1656,   250,   251,     0,     0,   507,   508,   252,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1657,     0,     0,     0,   509,
     510,   511,   512,     0,   245,     0,  1181,   253,     0,   344,
     345,   346,   254,   347,   348,     0,   349,     0,  1405,  1229,
     350,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,     0,   244,   247,   248,   457,     0,
     250,   251,  1459,  1455,     0,     0,   252,     0,     0,     0,
     246,     0,     0,   339,   247,   248,     0,     0,   250,   251,
       0,  1182,     0,     0,   252,     0,     0,     0,  1457,     0,
       0,   513,   514,     0,     0,   245,   253,     0,     0,     0,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1530,   253,     0,     0,     0,     0,   254,
       0,     0,     0,   246,     0,     0,     0,   247,   248,     0,
     249,   250,   251,     0,   458,   459,     0,   252,  1531,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,     0,   253,     0,   326,
    1183,   244,   254,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,     0,     0,     0,     0,   305,   306,   307,     0,     0,
     355,     0,     0,     0,     0,   327,   328,   329,     0,     0,
       0,   245,     0,     0,   330,     0,     0,     0,     0,   488,
       0,   244,     0,     0,     0,     0,   260,     0,     0,   331,
       0,   308,  1184,     0,     0,     0,     0,     0,   309,   246,
       0,   261,     0,   247,   248,     0,     0,   250,   251,     0,
       0,   316,     0,   252,     0,   262,  1185,     0,  1186,     0,
       0,   245,     0,  1187,     0,  1188,     0,  1548,   356,  1189,
    1190,     0,     0,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,   253,     0,     0,     0,     0,   254,   246,
       0,     0,     0,   247,   248,   263,   264,   250,   251,     0,
       0,     0,     0,   252,     0,     0,     0,   265,     0,     0,
     266,  1191,   267,     0,     0,     0,   317,     0,     0,     0,
       0,     0,     0,     0,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   253,     0,     0,   324,     0,   254,     0,
       0,     0,     0,     0,   318,     0,     0,     0,     0,     0,
     507,   508,   268,     0,   269,     0,     0,     0,   325,     0,
       0,     0,   351,   270,     0,     0,     0,     0,     0,     0,
       0,  1549,     0,     0,   509,   510,   511,   512,     0,   928,
     771,     0,     0,     0,     0,  1550,   518,   519,   520,   521,
     522,     0,     0,     0,     0,   401,   402,     0,     0,   271,
     523,   524,   525,   526,   527,   528,   529,   530,     0,     0,
     531,     0,  1177,     0,     0,   255,     0,     0,     0,   272,
       0,   772,     0,     0,     0,     0,   273,   274,     0,   275,
       0,   276,   277,     0,   278,   279,     0,   280,     0,     0,
     281,   282,     0,   283,     0,  1551,   513,   514,  1552,  1553,
       0,     0,     0,     0,     0,     0,   284,   285,     0,     0,
       0,   286,   773,   287,   288,     0,   289,     0,     0,     0,
       0,   290,     0,     0,     0,   291,   292,     0,   293,   294,
       0,   295,   296,   297,     0,     0,     0,   298,     0,     0,
       0,     4,   774,   299,     0,     0,     0,     0,     0,   775,
     776,     0,     0,     0,     0,  1554,     0,     0,  1555,     0,
     777,  1556,     0,     0,     0,     0,     5,     6,     0,   778,
     779,   780,   781,     0,   782,   783,     7,     0,  1557,     0,
    1558,   357,  1559,  1560,  1561,  1562,  1563,  1564,     0,     0,
       0,     0,     0,  1565,     0,     0,     0,  1566,  1567,     0,
       0,  1568,     0,     0,     0,     0,   890,   891,   892,   893,
     894,   895,   896,  1569,   897,   898,   899,   900,   901,     0,
       0,  1570,     0,     0,     0,     0,     0,     8,     0,     0,
       0,   300,     0,     0,     0,     0,     0,     0,  1406,   890,
     891,   892,   893,   894,   895,   896,     0,   897,   898,   899,
     900,   901,     0,     0,     0,     9,     0,  1571,    10,     0,
      11,     0,     0,     0,     0,     0,   606,    12,     0,    13,
       0,     0,     0,     0,    14,    15,    16,     0,     0,   517,
       0,    17,    18,    19,   518,   519,   520,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
     525,   526,   527,   528,   529,   530,     0,     0,   531,   518,
     519,   520,   521,   522,     0,     0,     0,     0,     0,     0,
    -460,  -460,  -460,   523,   524,   525,   526,   527,   528,   529,
     530,     0,     0,   531,  -460,  -460,  -460,  -460,   527,   528,
     529,   530,     0,     0,   531,   518,   519,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,   525,   526,   527,   528,   529,   530,     0,     0,   531,
     890,   891,   892,   893,   894,   895,   896,     0,   897,   898,
     899,   900,   901
};

static const yytype_int16 yycheck[] =
{
      17,   217,   209,   186,   154,   155,    83,   186,  1028,   216,
     217,   693,   304,   696,   274,   275,   112,   587,   114,   153,
     116,  1087,   118,   781,   120,    29,     6,   123,   124,     7,
      15,    22,    18,    19,  1028,    15,    16,    17,    18,   124,
     247,    15,    15,    17,    18,    19,    19,     6,    18,    19,
      18,    19,   914,   124,  1462,    28,    15,    16,    17,    18,
      19,     8,     9,    10,    11,    12,   273,    84,    30,    44,
     150,    15,    16,    17,    18,    22,    23,    24,    25,    26,
      27,    28,    29,     6,    85,    32,  1023,    15,    16,    17,
      18,    50,    15,    16,    17,    18,    19,    15,    19,   119,
     307,    19,    15,    15,    17,    18,    19,    19,  1361,    15,
      16,    17,    18,    19,   321,   164,    27,  1111,  1459,     4,
     327,   328,    34,    35,    36,    37,     3,    50,     8,     9,
      10,    11,    12,    15,    16,    17,    18,    19,    19,   208,
     124,  1230,    22,    23,    24,    25,    26,    27,    28,    29,
      15,  1462,    32,  1437,    19,    15,    35,   174,   117,    19,
      15,   368,    15,    75,    19,    44,    19,     3,   185,    34,
      35,    36,    37,    41,    15,   244,   253,  1518,    19,   169,
      35,   248,    15,    16,    17,    18,    19,   125,   338,   266,
     251,  1599,    18,    19,   117,     8,     9,    10,    11,    12,
     267,     3,    26,    27,   886,   272,  1490,   243,   175,    22,
      23,    24,    25,    26,    27,    28,    29,    15,    35,    32,
     287,    19,   135,  1476,    15,   163,   144,    44,    19,   296,
     189,   190,   191,   192,   193,   194,   195,   132,   305,    35,
     390,   308,    41,    34,    35,    36,    37,   324,    44,  1269,
     209,     0,    15,   212,   135,   224,    19,  1598,   392,   344,
     394,   395,   347,   397,   398,   266,   189,   190,   191,   192,
     193,   194,   195,    19,  1211,  1269,    41,    68,   345,   269,
     347,   301,   203,   242,     3,   254,   209,   150,  1599,   212,
     252,   144,     8,     9,    10,    11,    12,   798,   175,   404,
     801,   406,   803,   257,   172,   597,    22,    23,    24,    25,
      26,    27,    28,    29,   268,  1404,    32,   388,   389,   242,
       8,     9,    10,    11,    12,   405,   362,   238,    15,   402,
     391,   392,    19,   406,    22,    23,    24,    25,    26,    27,
      28,    29,   558,  1337,    32,   915,   172,    34,    35,    36,
      37,   558,   272,   273,   153,   154,   155,   564,    19,    20,
     344,   328,  1120,   347,   571,   414,    85,   416,   263,   418,
     419,   150,   632,     8,     9,    10,    11,    12,    19,   167,
     168,   246,   247,    44,   351,    80,  1406,    22,    23,    24,
      25,    26,    27,    28,    29,   602,   308,    32,   351,   403,
     417,    67,   609,    44,   611,   358,   423,   172,  1474,  1271,
     401,    20,  1406,    80,   431,   432,   433,   434,   435,   405,
     627,   401,   407,   993,   402,   405,   145,   401,   401,   404,
     447,   638,   150,   640,   407,   405,   643,   405,   405,   646,
     647,   160,   401,    19,    20,    19,   405,    15,    57,    58,
     135,    19,   137,   172,    19,   402,   716,   401,   718,   406,
     167,   168,   129,   130,   131,  1459,    34,    35,    36,    37,
     547,    42,    15,   401,   140,   682,    19,   737,   401,    44,
     146,    19,   405,   401,   135,   692,   137,   114,   401,   401,
      61,    34,    35,    36,    37,   401,   703,  1517,   114,   160,
     161,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   355,   356,  1274,  1200,   725,   401,
     727,   781,   149,  1517,  1518,   404,   406,    22,   735,   546,
      15,   176,   400,   609,    19,   611,   401,    32,  1604,   136,
     400,   401,   559,    19,   115,   333,   401,   807,   401,    34,
      35,    36,    37,   620,   621,     8,     9,    10,    11,    12,
     401,   628,   579,   630,   140,  1066,   193,  1068,   401,    22,
      23,    24,    25,    26,    27,    28,    29,    18,    19,    32,
     404,   648,   120,   371,   328,   329,   653,   404,   655,   606,
     657,    85,   659,   406,    44,   662,   663,   664,   123,   806,
    1282,   400,   400,   401,  1598,   176,  1325,    49,   404,   676,
     401,   152,   679,     8,     9,    10,    11,    12,    68,   363,
     770,   658,  1304,    19,    66,    19,   333,    22,    23,    24,
      25,    26,    27,    28,    29,   400,    15,    32,   401,   284,
      19,   328,    28,    29,   711,   183,    32,   244,   245,   404,
     910,   248,   249,   178,   179,    34,    35,    36,    37,   842,
     843,     3,   729,   730,   731,   310,   800,     3,   802,   848,
      20,     3,   406,   123,   124,   409,   713,     3,  1397,  1398,
    1399,  1400,   132,   975,    66,   406,   263,    19,   409,    13,
     406,    15,    16,    17,    18,    19,    20,    33,   194,     3,
     907,     3,    26,    27,    28,    29,    42,    31,   233,    45,
      38,    39,    40,   238,   401,   292,   293,     3,   406,    85,
      56,     8,     9,    10,    11,    12,    19,    20,   403,   936,
     937,   406,   939,    69,     3,    22,    23,    24,    25,    26,
      27,    28,    29,    79,     3,    32,   254,    19,    20,    85,
      43,   958,   959,    85,    19,   962,   247,   964,   965,    20,
     967,    54,   247,     3,   260,  1025,   973,  1059,     3,   919,
    1062,   406,    44,  1065,     3,   266,     3,   113,   130,   115,
     116,   133,    44,   115,   116,   992,     3,   123,     3,   239,
     997,   123,     3,   117,    55,     3,   292,   365,    45,   123,
      19,    20,    63,    64,    65,   301,    68,    29,   405,  1016,
      19,    20,   303,   401,   150,   151,   238,   834,   150,   836,
     282,   283,   360,   378,   379,    44,   162,     8,     9,    10,
      11,    12,   401,   401,    19,    20,   374,   375,    85,   935,
     401,    22,    23,    24,    25,    26,    27,    28,    29,   185,
     401,    32,   188,   401,   178,   179,   188,   401,   401,   401,
    1120,   123,   124,    15,   931,     3,   113,  1127,   247,    22,
     117,   118,    62,   120,   121,   122,   943,   414,   400,   416,
     127,   418,   419,   950,   169,   952,   377,   336,   198,   213,
     214,   215,   216,   217,   218,   312,   313,   314,   315,   316,
     396,   397,   398,   399,   240,   158,   230,     3,   240,   233,
     157,   221,    19,   237,   405,   162,   401,   241,   400,   229,
      18,    19,    20,    19,   234,   235,    13,    19,    15,    16,
      17,    18,    19,    20,   135,   384,   385,   386,   387,    26,
      27,   404,    29,   404,    31,   404,     8,     9,    10,    11,
      12,   405,   406,   406,    59,    60,     7,     8,     9,    10,
      22,    23,    24,    25,    26,    27,    28,    29,    19,    13,
      32,    15,    16,    17,    18,    19,    20,   239,    78,    18,
      19,    20,    26,    27,    19,    29,   114,    31,    68,   312,
     313,   314,   315,   400,    13,   400,    15,    16,    17,    18,
      19,    20,   404,   250,   251,   404,  1213,    26,    27,    28,
      29,   406,    31,   309,  1274,   405,   406,   113,  1204,  1205,
    1206,  1313,   401,    59,    60,   125,   126,    80,  1235,   400,
     117,     8,     9,    10,    11,    12,   123,  1187,   138,   139,
      20,   141,   405,   406,   270,    22,    23,    24,    25,    26,
      27,    28,    29,   246,   247,    32,   405,   406,  1075,  1352,
    1353,  1078,  1079,  1080,   400,  1082,    84,  1084,   427,   428,
      68,   155,  1089,   117,    75,  1092,    78,   401,    75,   123,
     255,  1098,  1289,   262,   408,   268,   266,   138,   139,   286,
     282,   178,   179,   289,   296,   266,   291,   304,   117,   302,
    1307,  1308,   253,  1310,   123,   400,   400,   404,   359,   113,
     400,  1318,   404,   400,  1321,  1182,   404,  1184,   114,    72,
      85,   400,   404,  1190,    77,    75,   213,   214,   215,   216,
     217,   218,    94,   404,   178,   179,   138,    75,   140,   141,
     400,   103,   400,   230,    85,  1212,   233,   109,   404,   111,
     237,   113,   404,   115,   241,   117,   400,   119,   405,   178,
     179,  1228,   309,   404,    85,    75,   119,   263,  1428,   213,
     214,   336,   337,   338,   339,   340,   341,   342,   343,   181,
     182,   183,   184,    78,  1391,    84,   230,    80,   400,   233,
     400,    75,    20,   237,   213,   214,   400,   241,     3,   152,
     140,   141,   400,     3,  1221,     3,   400,     3,  1225,  1226,
     150,   230,     3,   309,   233,  1232,     3,    19,   237,     3,
       3,     3,   241,   404,   320,   165,     3,     3,   170,    14,
     238,  1438,  1439,  1440,   174,   170,   176,   177,    26,   335,
    1257,    15,  1309,    29,    13,    15,    15,    16,    17,    18,
      19,    20,    27,    32,    19,     7,   406,    26,    27,     3,
      29,   400,    31,   400,   400,    19,  1482,   401,   133,    75,
     366,   367,    75,   400,   400,  1482,  1483,    50,   400,  1462,
     294,   163,   400,   400,  1434,    30,  1436,   172,   400,  1496,
     243,   114,   400,   172,  1501,  1502,  1503,  1374,  1505,   400,
      19,   400,   318,   243,    75,   400,   400,    75,     8,     9,
      10,    11,    12,    75,   401,    75,   134,  1334,  1335,  1336,
      68,   408,    22,    23,    24,    25,    26,    27,    28,    29,
     400,   400,    32,   400,   400,   400,   400,   400,   260,   400,
     402,  1358,  1549,   400,  1361,   400,   400,     3,   117,     3,
     400,  1558,  1559,  1560,   123,  1562,  1563,   401,   402,    19,
      15,  1568,    19,     3,   408,    15,   319,    13,  1575,    15,
      16,    17,    18,    19,    20,    19,  1443,  1584,  1585,   319,
      26,    27,   401,    29,    19,    31,    20,    15,    27,   408,
    1606,  1607,  1608,  1410,  1411,   402,  1413,     7,    15,  1606,
    1607,  1608,    18,  1663,   404,    20,  1613,  1614,  1615,   178,
     179,   406,  1479,   406,  1597,   401,  1599,  1624,   400,  1626,
    1487,   400,   400,   376,    27,   402,   128,   380,  1495,   400,
    1497,   406,   400,   400,    13,  1642,    15,    16,    17,    18,
      19,    20,   395,   400,   213,   214,   400,    26,    27,    28,
      29,   400,    31,   400,   400,   395,   400,    19,     3,  1476,
     405,   230,   405,   400,   233,     3,   400,   400,   237,   400,
     400,   117,   241,   400,   400,   400,   400,   123,     8,     9,
      10,    11,    12,   400,  1551,   400,   400,   400,  1555,   400,
    1557,   400,    22,    23,    24,    25,    26,    27,    28,    29,
     400,    68,    32,   400,   400,  1522,     8,     9,    10,    11,
      12,   400,   400,   400,   400,  1532,  1533,  1534,  1535,   400,
      22,    23,    24,    25,    26,    27,    28,    29,   400,   400,
      32,    15,   178,   179,     3,     3,   172,    27,   117,   172,
     406,   400,  1609,   170,   123,   402,   400,   400,   406,    19,
    1617,   196,   197,   198,   199,   200,   201,   202,   400,   204,
     205,   206,   207,   208,   400,   400,   400,   213,   214,  1636,
     400,   400,   309,  1640,   400,   400,  1643,    20,   400,   400,
      50,   400,    22,   400,   230,   400,   400,   233,   368,   401,
     400,   237,     3,     3,    15,   241,    15,   406,   402,   178,
     179,    15,    15,   404,  1671,     8,     9,    10,    11,    12,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    22,
      23,    24,    25,    26,    27,    28,    29,   405,   400,    32,
      22,   400,   401,  1650,   213,   214,     7,   404,   404,   408,
     404,   404,   400,   400,   400,   400,   405,   117,   172,   400,
     400,   230,    19,   406,   233,   253,   304,   400,   237,   400,
     402,     3,   241,     8,     9,    10,    11,    12,   404,     3,
     401,    15,   402,   250,   402,   211,   401,    22,    23,    24,
      25,    26,    27,    28,    29,   401,   401,    32,   401,     8,
       9,    10,    11,    12,   400,   400,   247,   400,   268,   400,
     353,   400,   402,    22,    23,    24,    25,    26,    27,    28,
      29,   400,   405,    32,     8,     9,    10,    11,    12,   189,
     190,   191,   192,   193,   194,   195,     7,   406,    22,    23,
      24,    25,    26,    27,    28,    29,   402,   248,    32,   209,
     248,   248,   212,   309,   304,   404,   354,   372,   268,     8,
       9,    10,    11,    12,   338,   401,   370,   402,   362,   400,
     336,   373,   408,    22,    23,    24,    25,    26,    27,    28,
      29,   400,   242,    32,     8,     9,    10,    11,    12,   400,
     400,   400,   400,   348,   353,   251,   869,   400,    22,    23,
      24,    25,    26,    27,    28,    29,   402,  1553,    32,     8,
       9,    10,    11,    12,   550,   364,  1121,  1040,  1200,  1043,
     383,  1110,  1227,    22,    23,    24,    25,    26,    27,    28,
      29,   993,   401,    32,     8,     9,    10,    11,    12,   408,
     842,  1271,   581,   390,  1517,  1597,   185,   776,    22,    23,
      24,    25,    26,    27,    28,    29,  1088,  1599,    32,     8,
       9,    10,    11,    12,   843,  1230,   848,  1413,  1335,  1075,
    1126,  1370,  1386,    22,    23,    24,    25,    26,    27,    28,
      29,   957,   402,    32,     8,     9,    10,    11,    12,  1431,
      -1,  1027,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    -1,
     402,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,     8,     9,    10,    11,
      12,    -1,    -1,    -1,    -1,   405,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,     8,     9,    10,    11,
      12,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,   402,
      32,     8,     9,    10,    11,    12,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   134,    50,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,     8,     9,    10,    11,
      12,    -1,    -1,     5,    -1,   156,    -1,    -1,    50,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,   169,    -1,
      32,     8,     9,    10,    11,    12,    -1,   402,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,     8,     9,    10,    11,
      12,   117,    -1,   402,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    -1,    -1,    -1,    -1,   117,    -1,    -1,   402,    81,
      82,    -1,    -1,    -1,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,    -1,   402,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,   189,   190,   191,   192,   193,   194,   195,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,   402,    65,
      -1,    -1,    -1,   209,    -1,    -1,   212,   189,   190,   191,
     192,   193,   194,   195,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,   402,   166,    41,    -1,   209,    -1,    45,
     212,    -1,    -1,    -1,    -1,    -1,   242,   243,    -1,   330,
     331,   332,    -1,   334,    -1,    -1,    -1,   113,   402,    -1,
      66,   117,   118,    -1,   345,   121,   122,    -1,    -1,    -1,
     242,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,   402,    -1,    45,    -1,   219,   220,    -1,
     222,   223,   224,   225,   226,   227,   228,    -1,    -1,    -1,
      -1,   157,    -1,    -1,    -1,    -1,   162,   113,   402,    -1,
      -1,   117,   118,    -1,   120,   121,   122,    -1,    -1,    -1,
     176,   127,    -1,    83,   405,    85,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,   402,    -1,    -1,    -1,   271,
     272,   273,   274,   275,   276,   277,   278,   279,    -1,    -1,
     156,   157,    -1,   113,    45,    -1,   162,   117,   118,    -1,
     402,   121,   122,    -1,    -1,   297,   298,   127,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   402,    -1,    -1,    -1,   321,
     322,   323,   324,    -1,    85,    -1,   120,   157,    -1,    72,
      73,    74,   162,    76,    77,    -1,    79,    -1,   400,   405,
      83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    45,   117,   118,     5,    -1,
     121,   122,   404,   400,    -1,    -1,   127,    -1,    -1,    -1,
     113,    -1,    -1,   299,   117,   118,    -1,    -1,   121,   122,
      -1,   175,    -1,    -1,   127,    -1,    -1,    -1,   400,    -1,
      -1,   393,   394,    -1,    -1,    85,   157,    -1,    -1,    -1,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   400,   157,    -1,    -1,    -1,    -1,   162,
      -1,    -1,    -1,   113,    -1,    -1,    -1,   117,   118,    -1,
     120,   121,   122,    -1,    81,    82,    -1,   127,   400,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   157,    -1,   345,
     264,    45,   162,    -1,    -1,    -1,    -1,    -1,    -1,   405,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,    -1,   325,   326,   327,    -1,    -1,
      74,    -1,    -1,    -1,    -1,   381,   382,   383,    -1,    -1,
      -1,    85,    -1,    -1,   390,    -1,    -1,    -1,    -1,   166,
      -1,    45,    -1,    -1,    -1,    -1,    50,    -1,    -1,   405,
      -1,   361,   326,    -1,    -1,    -1,    -1,    -1,   368,   113,
      -1,    65,    -1,   117,   118,    -1,    -1,   121,   122,    -1,
      -1,   312,    -1,   127,    -1,    79,   350,    -1,   352,    -1,
      -1,    85,    -1,   357,    -1,   359,    -1,    65,   142,   363,
     364,    -1,    -1,    -1,    -1,   405,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,   162,   113,
      -1,    -1,    -1,   117,   118,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,   131,    -1,    -1,
     134,   405,   136,    -1,    -1,    -1,   377,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   157,    -1,    -1,   134,    -1,   162,    -1,
      -1,    -1,    -1,    -1,   405,    -1,    -1,    -1,    -1,    -1,
     297,   298,   176,    -1,   178,    -1,    -1,    -1,   156,    -1,
      -1,    -1,   405,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,   321,   322,   323,   324,    -1,    19,
       3,    -1,    -1,    -1,    -1,   183,     8,     9,    10,    11,
      12,    -1,    -1,    -1,    -1,    18,    19,    -1,    -1,   223,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    -1,    19,    -1,    -1,   405,    -1,    -1,    -1,   243,
      -1,    44,    -1,    -1,    -1,    -1,   250,   251,    -1,   253,
      -1,   255,   256,    -1,   258,   259,    -1,   261,    -1,    -1,
     264,   265,    -1,   267,    -1,   243,   393,   394,   246,   247,
      -1,    -1,    -1,    -1,    -1,    -1,   280,   281,    -1,    -1,
      -1,   285,    85,   287,   288,    -1,   290,    -1,    -1,    -1,
      -1,   295,    -1,    -1,    -1,   299,   300,    -1,   302,   303,
      -1,   305,   306,   307,    -1,    -1,    -1,   311,    -1,    -1,
      -1,    45,   115,   317,    -1,    -1,    -1,    -1,    -1,   122,
     123,    -1,    -1,    -1,    -1,   303,    -1,    -1,   306,    -1,
     133,   309,    -1,    -1,    -1,    -1,    70,    71,    -1,   142,
     143,   144,   145,    -1,   147,   148,    80,    -1,   326,    -1,
     328,   405,   330,   331,   332,   333,   334,   335,    -1,    -1,
      -1,    -1,    -1,   341,    -1,    -1,    -1,   345,   346,    -1,
      -1,   349,    -1,    -1,    -1,    -1,   196,   197,   198,   199,
     200,   201,   202,   361,   204,   205,   206,   207,   208,    -1,
      -1,   369,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,   405,    -1,    -1,    -1,    -1,    -1,    -1,   210,   196,
     197,   198,   199,   200,   201,   202,    -1,   204,   205,   206,
     207,   208,    -1,    -1,    -1,   159,    -1,   405,   162,    -1,
     164,    -1,    -1,    -1,    -1,    -1,   239,   171,    -1,   173,
      -1,    -1,    -1,    -1,   178,   179,   180,    -1,    -1,     3,
      -1,   185,   186,   187,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
       8,     9,    10,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    32,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
     196,   197,   198,   199,   200,   201,   202,    -1,   204,   205,
     206,   207,   208
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     4,   411,   412,    45,    70,    71,    80,   131,   159,
     162,   164,   171,   173,   178,   179,   180,   185,   186,   187,
     436,   437,   438,   439,   440,   441,   492,   493,   494,   495,
     497,   498,   499,   502,   505,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   604,   641,   642,
     643,   644,   645,   646,   647,   648,   670,   672,     0,     3,
      33,    42,    45,    56,    69,    79,    85,   113,   115,   116,
     123,   150,   151,   162,   185,   188,   240,   400,   413,   425,
     431,   434,   435,   444,   446,   449,   451,   454,   456,   457,
     459,   460,   467,   477,   478,   534,   536,   538,   551,   590,
     593,   605,   607,   608,   609,   650,   651,   652,   653,   661,
     662,   671,   673,   674,   675,    19,    20,    44,   532,    20,
      55,    63,    64,    65,   442,     3,   532,   532,   160,   161,
     532,   532,     3,   150,   140,   141,   150,   165,   174,   176,
     177,   243,   319,   395,   150,   150,    78,   138,   140,   141,
     181,   182,   183,   184,    13,    15,    16,    17,    18,    19,
      20,    26,    27,    29,    31,   117,   123,   178,   179,   213,
     214,   230,   233,   237,   241,   401,   408,   556,   569,   574,
     577,   578,   579,   581,   583,    35,    44,   404,   521,   532,
      19,    19,    20,    43,    54,   423,   427,    67,   140,   146,
      42,    61,   115,   176,   432,   433,    49,    66,    80,    19,
     114,    19,    85,   152,   140,   423,   583,    19,    19,    19,
     135,   461,   461,   404,    72,    77,   119,   152,   243,   319,
     376,   380,   395,   461,    45,    85,   113,   117,   118,   120,
     121,   122,   127,   157,   162,   405,   462,   480,   461,   461,
      50,    65,    79,   119,   120,   131,   134,   136,   176,   178,
     187,   223,   243,   250,   251,   253,   255,   256,   258,   259,
     261,   264,   265,   267,   280,   281,   285,   287,   288,   290,
     295,   299,   300,   302,   303,   305,   306,   307,   311,   317,
     405,   462,   461,    83,   320,   325,   326,   327,   361,   368,
     405,   462,   610,   611,   612,   461,   312,   377,   405,   462,
     461,    41,    66,   120,   134,   156,   345,   381,   382,   383,
     390,   405,   462,   487,   489,   490,   461,    65,   176,   299,
     405,   462,   663,   666,    72,    73,    74,    76,    77,    79,
      83,   405,   445,   448,   462,    74,   142,   405,   447,   462,
      35,    44,   404,   481,   442,     3,     3,   442,    66,   443,
       3,     3,   532,   532,     3,    35,    44,   404,   484,    85,
     507,    19,    20,    44,   533,     3,   175,   503,     3,     3,
     500,   506,   254,    19,   120,   183,   360,   374,   375,   496,
     496,    18,    19,   417,   419,   419,     3,     3,     3,     3,
       3,     3,     3,     3,    15,    19,    28,   401,   407,   573,
     575,   581,    29,   401,   238,    27,   238,    15,   407,   576,
     583,   401,   401,   401,   401,   401,   401,   215,   216,   217,
     218,   563,   583,    15,    16,    17,    19,   401,   556,   557,
     561,   564,   565,   567,   569,   570,   571,     5,    81,    82,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   135,   166,   219,
     220,   222,   223,   224,   225,   226,   227,   228,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   297,   298,   321,
     322,   323,   324,   393,   394,   582,   676,     3,     8,     9,
      10,    11,    12,    22,    23,    24,    25,    26,    27,    28,
      29,    32,    45,    46,    47,    48,    49,    50,    51,    52,
      53,   429,   430,    15,   522,     3,    22,    62,   426,    44,
     404,   428,    15,    19,    34,    35,    36,    37,    68,   401,
     414,   416,   481,   414,    20,    57,    58,   400,   414,   415,
     415,   169,   158,   455,    15,    17,    18,    19,   135,   401,
     450,   535,    19,   452,   484,   400,   552,   537,   135,    19,
     404,   404,   458,   404,   481,    19,   466,   114,   414,   416,
     479,    44,    68,   123,   124,   132,   239,   463,   548,    68,
     463,    68,   423,   484,   400,   400,   404,   404,   169,   269,
     309,    80,   400,    20,   270,   423,   416,    68,    84,   176,
     284,   310,   155,   416,   400,   414,   417,   606,   172,   417,
      75,   257,   268,    75,   255,   262,   272,   273,   266,   268,
     282,   283,   282,   286,   416,   289,   291,   296,    85,   266,
     119,   301,   266,   304,   302,   416,   253,   312,   313,   314,
     315,   316,   544,   400,   400,   404,   359,   450,   416,   113,
     414,   416,   114,   400,   404,   404,    85,   378,   379,   400,
     404,   414,    75,   659,   660,   423,   404,   124,   388,   389,
     654,   414,   414,    75,   251,   391,   392,   400,   400,   404,
     404,   309,   419,    85,   400,   194,   260,   292,   301,   396,
     397,   398,   399,   404,   669,    85,   416,    75,   416,    78,
      80,    84,   400,   400,   400,    75,    20,   424,   400,   400,
      78,   125,   126,   138,   139,   141,   482,   483,     3,   443,
       3,   414,     3,     3,     3,   125,   163,   485,   486,     3,
      19,     3,   533,     3,     3,     3,    85,   145,   160,   172,
     504,     3,    44,    85,   115,   122,   123,   133,   142,   143,
     144,   145,   147,   148,   419,   501,   548,     3,    19,    85,
     115,   116,   123,   150,   188,   240,   496,   496,   496,   649,
      44,   496,    44,   496,     3,     3,    29,   403,   421,   170,
     170,   575,   575,   583,   575,   575,    14,    15,   583,   587,
     588,    26,   238,    27,   576,   576,    29,   583,   583,   583,
     583,   583,    19,    15,   406,   402,   406,    28,   559,   560,
     563,   583,     7,   406,   409,    26,    27,   404,   406,   409,
     583,   583,   583,   583,   583,   583,   583,   583,   583,   583,
     583,   583,   583,   198,   221,   229,   234,   235,   405,   406,
       3,   583,   423,   429,   400,   415,   583,   414,    59,    60,
      59,    60,    38,    39,    40,   414,   474,   475,   583,   400,
     196,   197,   198,   199,   200,   201,   202,   204,   205,   206,
     207,   208,   539,   540,   541,   543,   550,    41,   153,   154,
     155,   400,   453,   400,   401,   549,   545,   550,    19,   150,
     405,   450,   468,   471,   472,    35,   414,   465,    19,   543,
     546,   133,   583,   465,   465,   592,    75,    75,   416,    75,
     308,   416,   400,    50,   400,   400,   414,   246,   247,   416,
     294,   416,   163,   417,   400,   400,    30,   418,    41,   172,
     400,   414,   172,   414,    75,    75,   414,    75,   132,   263,
     414,   414,   416,    75,   400,   114,   400,   416,   400,   416,
     263,   292,   293,   416,   466,   416,   400,   400,   416,   416,
     416,   400,   172,   318,   400,   416,   416,    68,   414,   613,
      19,   400,   400,   400,   414,   472,   400,   488,   489,   400,
     400,   400,   414,   400,   400,   400,   114,   149,   193,   491,
     416,   466,    20,   417,   417,   260,   664,   665,   668,   400,
     414,   414,   416,   416,   416,   414,   417,   419,   420,   405,
     406,     3,   405,   406,     3,    19,    15,   419,    19,    19,
     546,   420,    19,     3,    20,   640,   640,     3,    19,   113,
     263,   309,   320,   335,   366,   367,   496,   640,   496,   640,
     414,   417,    15,    15,   402,   406,   402,    27,   406,   406,
     406,   402,   406,   402,   406,   583,   583,     7,   406,   406,
     402,    18,   401,   557,   565,    15,    18,   570,   401,   430,
     400,   405,   402,    20,   400,   472,   400,   402,    19,   203,
     400,   404,   414,   417,   402,   540,   542,   550,   419,    27,
     470,   406,   128,   464,   416,   120,   250,   251,   405,   462,
     591,   414,   414,   400,   414,   400,   400,   416,   400,   400,
     400,   400,   416,   400,   416,   400,    19,   606,   414,   414,
     400,   414,   400,   414,   414,   400,   414,   400,   400,   400,
     400,   400,   414,   450,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   414,    19,   545,   547,
     414,   120,   175,   264,   326,   350,   352,   357,   359,   363,
     364,   405,   614,   621,   624,   400,   400,   404,   658,   405,
     400,   400,   414,   135,   137,   135,   137,   405,   400,   400,
     418,   417,   243,   362,   664,    19,    50,   117,   189,   190,
     191,   192,   193,   194,   195,   209,   212,   242,   243,   405,
     579,   580,   584,   589,   667,    68,   483,   486,     3,     3,
     450,   450,    15,   450,   640,     3,   640,     3,   172,   172,
     587,    27,   583,   583,   583,   583,   583,   406,   402,   559,
     583,   560,   562,   583,   583,   406,   583,   541,   553,   554,
     589,   400,   402,   170,    20,   420,   469,   471,    80,   129,
     130,   131,   594,   598,   594,   599,   417,   603,   400,   406,
     400,   400,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   633,   639,   416,   328,   329,   363,
     328,   416,   615,   309,   419,   617,   618,   616,   365,   416,
     400,   368,   634,   657,   488,   400,    20,   422,   422,   422,
     422,   418,   416,   414,    22,   401,    22,   401,   583,   583,
     583,   544,   416,   582,   676,   400,   583,   585,   586,   414,
       3,     3,    15,    15,   402,   402,   402,   402,   406,   583,
     402,   406,    15,   402,   405,   589,   542,    15,   420,   472,
     473,   404,   597,    30,   252,   600,   414,   472,   635,   414,
     414,   416,   414,   404,   450,   404,   404,   414,   414,   404,
     247,   266,   303,   377,   405,   656,   491,   400,   400,   400,
     400,   583,   588,   583,   579,   400,   210,   404,   400,   400,
      22,    22,    32,   406,   400,   507,   507,   583,   402,     7,
      28,   562,   405,   172,   597,   596,    19,   423,   253,   601,
     400,   636,   404,   638,   626,   619,   626,   622,   167,   168,
     333,   476,   414,   304,   336,   384,   385,   386,   387,   655,
     400,   491,   491,   491,   491,   400,   402,   400,   676,   404,
     555,   589,   572,   583,   583,   123,   178,   179,   233,   238,
     586,     3,     3,   402,   401,   402,     7,   402,    15,   136,
     244,   245,   248,   249,   405,   595,   417,   250,   602,   634,
     637,   405,   419,   627,   628,   175,   328,   351,   405,   620,
     405,   169,   330,   331,   332,   334,   345,   405,   487,   623,
     414,   414,   414,   247,   416,   402,   553,   211,     6,    15,
      16,    17,   401,   405,   556,   558,   561,   564,   566,   568,
     400,   400,   401,   401,   401,   401,   400,   559,   562,   400,
     416,   246,   247,   247,   415,   414,   400,   416,    65,   169,
     183,   243,   246,   247,   303,   306,   309,   326,   328,   330,
     331,   332,   333,   334,   335,   341,   345,   346,   349,   361,
     369,   405,   487,   625,   351,   358,   629,   416,   414,   416,
     400,   414,   414,   414,   268,   144,   414,   124,   344,   347,
     400,   353,   400,   405,   555,   553,   583,     7,   403,   406,
     583,   583,   583,   583,   406,   402,   248,   248,   248,   309,
     414,   404,   416,   354,   333,   371,   476,   304,   416,   372,
     416,   414,   414,   414,   268,   414,   144,   414,   336,   337,
     338,   339,   340,   341,   342,   343,   370,   124,   344,   347,
     338,   414,   362,   336,   400,   414,   400,   414,   414,   402,
     401,   558,   553,   566,   402,   402,   402,   402,   559,   415,
     415,   415,   416,   630,   355,   356,   414,   414,   414,   247,
     416,   373,   414,   414,   416,   416,   414,   416,   583,   400,
     400,   400,   400,   402,   405,   417,   632,   353,   416,   348,
     631,   400
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   410,   411,   411,   412,   412,   413,   414,   414,   414,
     415,   415,   415,   416,   416,   416,   416,   416,   416,   417,
     417,   418,   418,   419,   420,   420,   421,   421,   422,   423,
     423,   424,   424,   425,   426,   426,   427,   427,   427,   428,
     428,   429,   429,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   431,   431,   432,   432,   432,   432,   433,   433,
     433,   433,   433,   433,   434,   435,   435,   435,   435,   436,
     437,   438,   439,   440,   441,   442,   442,   443,   443,   443,
     444,   445,   445,   445,   445,   445,   445,   445,   446,   447,
     447,   448,   449,   449,   450,   450,   450,   450,   450,   451,
     452,   452,   453,   453,   453,   453,   454,   455,   455,   456,
     457,   458,   458,   459,   459,   460,   460,   461,   461,   461,
     461,   462,   462,   462,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   463,   463,   463,   463,   464,
     464,   464,   464,   464,   465,   465,   466,   467,   467,   468,
     469,   469,   469,   470,   470,   471,   472,   472,   473,   474,
     475,   476,   476,   477,   478,   478,   478,   479,   480,   481,
     481,   481,   482,   482,   483,   483,   483,   483,   483,   483,
     484,   484,   484,   485,   485,   486,   486,   487,   487,   488,
     488,   489,   489,   490,   491,   491,   491,   491,   491,   491,
     492,   493,   494,   495,   496,   496,   497,   498,   499,   500,
     500,   500,   500,   500,   500,   500,   500,   500,   500,   500,
     500,   500,   501,   501,   501,   502,   503,   503,   503,   503,
     504,   504,   504,   505,   506,   506,   506,   506,   506,   506,
     506,   506,   506,   507,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   521,
     521,   522,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   532,   532,   533,   533,   533,   535,   534,
     537,   536,   538,   539,   539,   539,   539,   539,   539,   539,
     539,   539,   539,   539,   539,   539,   540,   541,   541,   542,
     542,   543,   544,   544,   544,   544,   545,   546,   546,   547,
     547,   548,   549,   549,   550,   552,   551,   553,   553,   554,
     554,   555,   555,   556,   557,   557,   557,   557,   557,   557,
     558,   558,   558,   558,   558,   559,   560,   560,   560,   561,
     561,   562,   563,   563,   563,   563,   564,   564,   564,   565,
     565,   565,   565,   566,   566,   566,   566,   567,   567,   568,
     568,   569,   570,   570,   570,   570,   571,   571,   572,   572,
     572,   573,   573,   574,   574,   575,   575,   575,   575,   575,
     576,   576,   576,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   578,   578,   579,   580,   581,   582,
     582,   582,   582,   582,   582,   582,   582,   582,   582,   583,
     583,   583,   583,   583,   583,   583,   583,   583,   583,   583,
     583,   583,   583,   583,   583,   583,   583,   583,   583,   583,
     583,   583,   583,   583,   583,   583,   583,   583,   583,   583,
     583,   583,   583,   583,   583,   583,   583,   584,   584,   584,
     584,   584,   584,   585,   586,   586,   586,   587,   587,   588,
     588,   589,   589,   589,   589,   589,   589,   589,   589,   589,
     589,   589,   589,   589,   589,   589,   589,   589,   590,   591,
     591,   591,   591,   592,   592,   593,   594,   595,   595,   595,
     595,   595,   595,   596,   596,   597,   597,   598,   599,   600,
     600,   600,   601,   601,   602,   602,   603,   604,   605,   606,
     606,   606,   607,   607,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   608,   609,   609,   610,   610,   610,
     610,   610,   610,   610,   610,   610,   611,   612,   613,   613,
     614,   614,   614,   614,   614,   614,   614,   614,   614,   615,
     614,   616,   614,   614,   614,   614,   617,   617,   618,   619,
     619,   620,   620,   620,   621,   621,   622,   622,   623,   623,
     623,   623,   623,   623,   623,   623,   623,   623,   623,   624,
     625,   625,   625,   625,   625,   625,   625,   625,   625,   625,
     625,   625,   625,   625,   625,   625,   625,   625,   625,   625,
     625,   625,   625,   625,   625,   625,   625,   625,   625,   625,
     625,   625,   625,   625,   625,   625,   625,   625,   625,   626,
     626,   627,   628,   629,   629,   629,   630,   630,   631,   631,
     632,   633,   634,   634,   636,   635,   637,   637,   638,   638,
     639,   640,   640,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   649,   649,   649,   649,   649,   649,   649,   649,
     650,   651,   651,   651,   651,   651,   652,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     654,   654,   654,   655,   655,   655,   655,   655,   656,   656,
     656,   656,   656,   656,   656,   657,   657,   658,   658,   659,
     660,   661,   662,   662,   662,   662,   662,   663,   664,   664,
     664,   665,   665,   666,   666,   666,   666,   666,   666,   666,
     666,   666,   667,   667,   668,   668,   669,   669,   670,   671,
     671,   671,   671,   671,   671,   671,   671,   671,   671,   671,
     671,   671,   671,   671,   671,   671,   672,   672,   672,   672,
     672,   672,   672,   672,   672,   672,   672,   672,   672,   672,
     672,   672,   672,   672,   672,   672,   672,   672,   672,   672,
     672,   672,   672,   672,   672,   672,   672,   672,   672,   672,
     672,   672,   672,   672,   672,   672,   672,   672,   672,   672,
     672,   672,   672,   673,   673,   673,   673,   673,   673,   673,
     673,   673,   674,   674,   674,   674,   675,   675,   675,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     2,     5,     1,     3,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     0,     1,
       1,     0,     2,     2,     1,     1,     2,     2,     1,     1,
       1,     1,     0,     4,     2,     0,     1,     2,     1,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     1,     1,     1,     1,     2,     4,
       3,     3,     3,     3,     3,     3,     4,     3,     3,     4,
       5,     3,     3,     4,     2,     0,     1,     0,     1,     2,
       2,     2,     3,     2,     3,     3,     3,     5,     2,     3,
       3,     3,     4,     5,     1,     3,     1,     1,     1,     4,
       0,     2,     2,     2,     1,     1,     5,     0,     1,     3,
       3,     0,     8,     1,     4,     1,     1,     0,     1,     2,
       3,     0,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     4,     2,     3,     2,     2,     1,     1,     1,     0,
       2,     2,     2,     2,     2,     1,     1,     3,     3,     0,
       1,     1,     2,     0,     1,     3,     1,     3,     1,     0,
       2,     2,     2,     2,     3,     3,     3,     0,     3,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     1,     4,     1,     0,
       3,     4,     1,     2,     0,     5,     5,     5,     5,     4,
       3,     3,     4,     5,     1,     0,     3,     4,     4,     0,
       2,     3,     3,     3,     2,     2,     2,     2,     3,     3,
       2,     2,     1,     1,     1,     4,     0,     3,     3,     3,
       0,     1,     1,     4,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     0,     2,     9,     9,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     1,     1,
       3,     0,     1,     3,     3,     3,     3,     4,     4,     4,
       4,     2,     1,     1,     1,     1,     0,     1,     0,     4,
       0,     4,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     2,     4,     0,     5,     0,     1,     1,
       2,     1,     3,     1,     1,     1,     1,     1,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     3,     1,     5,
      11,     1,     1,     1,     2,     1,     7,     9,     7,     1,
       1,     1,     3,     1,     1,     1,     3,     1,     3,     1,
       3,     3,     1,     2,     2,     6,     1,     3,     0,     4,
       3,     1,     3,     3,     3,     2,     2,     2,     2,     0,
       2,     2,     0,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     1,     1,     5,     7,     0,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     4,     1,     1,     1,     1,     2,
       1,     2,     3,     3,     6,     3,     3,     3,     3,     5,
       6,     6,     6,     6,     4,     8,     4,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     3,     1,     3,     0,
       1,     4,     6,     4,     3,     3,     5,     5,     4,     6,
       3,     2,     5,     5,     8,     8,     8,     8,     2,     1,
       2,     2,     2,     0,     3,     5,     0,     2,     4,     4,
       4,     2,     2,     0,     3,     0,     3,     3,     2,     0,
       2,     2,     0,     2,     0,     2,     4,     5,     2,     0,
       3,     3,     3,     3,     5,     6,     4,     6,     4,     6,
       4,     4,     5,     6,     3,     3,     4,     5,     5,     5,
       5,     5,     4,     4,     5,     5,     5,     5,     5,     5,
       5,     5,     6,     6,     6,     5,     6,     8,     6,     6,
       5,     6,     4,     5,     4,     5,     4,     5,     4,     5,
       5,     5,     5,     6,     5,     5,     6,     5,     5,     5,
       4,     4,     6,     3,     2,     3,     3,     1,     2,     3,
       2,     4,     3,     2,     3,     1,     2,     4,     0,     3,
       2,     1,     2,     3,     3,     3,     3,     2,     3,     0,
       5,     0,     5,     2,     2,     1,     4,     1,     1,     0,
       3,     2,     2,     2,     5,     2,     0,     3,     0,     2,
       2,     3,     2,     2,     3,     2,     2,     2,     1,     3,
       2,     2,     2,     2,     2,     2,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     2,     3,
       2,     3,     3,     4,     4,     2,     2,     2,     3,     3,
       3,     2,     3,     3,     3,     4,     2,     2,     1,     0,
       2,     3,     1,     0,     1,     2,     0,     2,     0,     1,
       3,     0,     0,     2,     0,     3,     0,     3,     0,     3,
       3,     1,     0,     4,     6,     6,     6,     7,     6,     7,
       5,     0,     2,     3,     2,     3,     3,     2,     3,     2,
       2,     3,     3,     5,     4,     4,     2,     3,     3,     4,
       4,     4,     5,     5,     4,     3,     4,     4,     4,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2,
       2,     2,     3,     3,     4,     0,     3,     0,     3,     0,
       3,     2,     3,     3,     5,     5,     4,     2,     3,     3,
       3,     1,     2,     4,     3,     3,     3,     2,     2,     2,
       2,     2,     1,     3,     0,     2,     0,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     1,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 728 "cf-parse.y" /* yacc.c:1646  */
    { return 0; }
#line 3790 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 729 "cf-parse.y" /* yacc.c:1646  */
    { return 0; }
#line 3796 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 742 "cf-parse.y" /* yacc.c:1646  */
    {
     struct f_val *val = cfg_alloc(sizeof(struct f_val));
     *val = f_eval((yyvsp[-1].x), cfg_mem);
     if (val->type == T_RETURN) cf_error("Runtime error");
     cf_define_symbol((yyvsp[-3].s), SYM_CONSTANT | val->type, val);
   }
#line 3807 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 752 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = f_eval_int((yyvsp[-1].x)); }
#line 3813 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 753 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != (SYM_CONSTANT | T_INT)) cf_error("Number expected");
     (yyval.i) = SYM_VAL((yyvsp[0].s)).i; }
#line 3821 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 760 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (u32) (yyvsp[-1].i) * 1000000; }
#line 3827 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 761 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (u32) (yyvsp[-1].i) * 1000; }
#line 3833 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 762 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (u32) (yyvsp[-1].i) * 1; }
#line 3839 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 770 "cf-parse.y" /* yacc.c:1646  */
    {(yyval.i) = !!(yyvsp[0].i); }
#line 3845 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 771 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 3851 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 772 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 3857 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 773 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 3863 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 774 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 3869 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 775 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 3875 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 782 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != (SYM_CONSTANT | T_IP)) cf_error("IP address expected");
     (yyval.a) = SYM_VAL((yyvsp[0].s)).px.ip;
   }
#line 3884 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 789 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = NULL; }
#line 3890 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 790 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = if_get_by_name((yyvsp[0].s)->name); }
#line 3896 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 794 "cf-parse.y" /* yacc.c:1646  */
    {
     if (!ip_is_prefix((yyvsp[-1].a), (yyvsp[0].i))) cf_error("Invalid prefix");
     (yyval.px).addr = (yyvsp[-1].a); (yyval.px).len = (yyvsp[0].i);
   }
#line 3905 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 802 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.px).addr = (yyvsp[0].a); (yyval.px).len = BITS_PER_IP_ADDRESS; }
#line 3911 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 806 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].i) < 0 || (yyvsp[0].i) > BITS_PER_IP_ADDRESS) cf_error("Invalid prefix length %d", (yyvsp[0].i));
     (yyval.i) = (yyvsp[0].i);
   }
#line 3920 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 810 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.i) = ipa_masklen((yyvsp[0].a));
     if ((yyval.i) < 0) cf_error("Invalid netmask %I", (yyvsp[0].a));
   }
#line 3929 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 817 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.time) = tm_parse_datetime((yyvsp[0].t));
     if (!(yyval.time))
       cf_error("Invalid date and time");
   }
#line 3939 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 826 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != (SYM_CONSTANT | T_STRING)) cf_error("String expected");
     (yyval.t) = SYM_VAL((yyvsp[0].s)).s;
   }
#line 3948 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 833 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = (yyvsp[0].t); }
#line 3954 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 834 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = NULL; }
#line 3960 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 840 "cf-parse.y" /* yacc.c:1646  */
    {
    struct log_config *c = cfg_allocz(sizeof(struct log_config));
    c->fh = (yyvsp[-2].g);
    c->mask = (yyvsp[-1].i);
    add_tail(&new_config->logfiles, &c->n);
  }
#line 3971 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 849 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = (yyvsp[0].t); }
#line 3977 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 850 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = bird_name; }
#line 3983 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 854 "cf-parse.y" /* yacc.c:1646  */
    {
     FILE *f = tracked_fopen(new_config->pool, (yyvsp[0].t), "a");
     if (!f) cf_error("Unable to open log file `%s': %m", (yyvsp[0].t));
     (yyval.g) = f;
   }
#line 3993 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 859 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.g) = NULL; new_config->syslog_name = (yyvsp[0].t); }
#line 3999 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 860 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.g) = stderr; }
#line 4005 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 864 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 4011 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 865 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 4017 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 869 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1 << (yyvsp[0].i); }
#line 4023 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 870 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (1 << (yyvsp[0].i)); }
#line 4029 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 874 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_DEBUG[0]; }
#line 4035 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 875 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_TRACE[0]; }
#line 4041 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 876 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_INFO[0]; }
#line 4047 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 877 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_REMOTE[0]; }
#line 4053 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 878 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_WARN[0]; }
#line 4059 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 879 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_ERR[0]; }
#line 4065 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 880 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_AUTH[0]; }
#line 4071 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 881 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_FATAL[0]; }
#line 4077 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 882 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_BUG[0]; }
#line 4083 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 888 "cf-parse.y" /* yacc.c:1646  */
    { new_config->proto_default_mrtdump = (yyvsp[-1].i); }
#line 4089 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 889 "cf-parse.y" /* yacc.c:1646  */
    {
     FILE *f = tracked_fopen(new_config->pool, (yyvsp[-1].t), "a");
     if (!f) cf_error("Unable to open MRTDump file '%s': %m", (yyvsp[-1].t));
     new_config->mrtdump_file = fileno(f);
   }
#line 4099 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 899 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_route; }
#line 4105 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 900 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_proto; }
#line 4111 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 901 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_base; }
#line 4117 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 902 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_log; }
#line 4123 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 905 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-1].tf) = (struct timeformat){(yyvsp[0].t), NULL, 0}; }
#line 4129 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 906 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-3].tf) = (struct timeformat){(yyvsp[-2].t), (yyvsp[0].t), (yyvsp[-1].i)}; }
#line 4135 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 907 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){"%T", "%F", 20*3600}; }
#line 4141 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 908 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){"%F %T", NULL, 0}; }
#line 4147 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 909 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){NULL, NULL, 0}; }
#line 4153 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 910 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){"%d-%m-%Y %T", NULL, 0}; }
#line 4159 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 920 "cf-parse.y" /* yacc.c:1646  */
    { new_config->latency_debug = (yyvsp[0].i); }
#line 4165 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 921 "cf-parse.y" /* yacc.c:1646  */
    { new_config->latency_limit = (yyvsp[0].i32); }
#line 4171 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 922 "cf-parse.y" /* yacc.c:1646  */
    { new_config->watchdog_warning = (yyvsp[0].i32); }
#line 4177 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 923 "cf-parse.y" /* yacc.c:1646  */
    { new_config->watchdog_timeout = ((yyvsp[0].i32) + 999999) TO_S; }
#line 4183 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 932 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig((yyvsp[-2].t), RECONFIG_HARD, (yyvsp[-1].i)); }
#line 4189 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 935 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig((yyvsp[-2].t), RECONFIG_SOFT, (yyvsp[-1].i)); }
#line 4195 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 942 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig_confirm(); }
#line 4201 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 945 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig_undo(); }
#line 4207 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 948 "cf-parse.y" /* yacc.c:1646  */
    { cmd_check_config((yyvsp[-1].t)); }
#line 4213 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 951 "cf-parse.y" /* yacc.c:1646  */
    { cmd_shutdown(); }
#line 4219 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 954 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = NULL; }
#line 4225 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 959 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4231 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 960 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = UNIX_DEFAULT_CONFIGURE_TIMEOUT; }
#line 4237 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 961 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 4243 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 969 "cf-parse.y" /* yacc.c:1646  */
    { this_proto = krt_init_config((yyvsp[-1].i)); }
#line 4249 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 974 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->persist = (yyvsp[0].i); }
#line 4255 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 975 "cf-parse.y" /* yacc.c:1646  */
    {
      /* Scan time of 0 means scan on startup only */
      THIS_KRT->scan_time = (yyvsp[0].i);
   }
#line 4264 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 979 "cf-parse.y" /* yacc.c:1646  */
    {
      THIS_KRT->learn = (yyvsp[0].i);
#ifndef KRT_ALLOW_LEARN
      if ((yyvsp[0].i))
	cf_error("Learning of kernel routes not supported in this configuration");
#endif
   }
#line 4276 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 986 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->devroutes = (yyvsp[0].i); }
#line 4282 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 987 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->graceful_restart = (yyvsp[0].i); }
#line 4288 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 988 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->merge_paths = (yyvsp[0].i) ? KRT_DEFAULT_ECMP_LIMIT : 0; }
#line 4294 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 989 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->merge_paths = (yyvsp[-2].i) ? (yyvsp[0].i) : 0; if (((yyvsp[0].i) <= 0) || ((yyvsp[0].i) > 255)) cf_error("Merge paths limit must be in range 1-255"); }
#line 4300 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 995 "cf-parse.y" /* yacc.c:1646  */
    { this_proto = kif_init_config((yyvsp[-1].i)); }
#line 4306 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1000 "cf-parse.y" /* yacc.c:1646  */
    {
      /* Scan time of 0 means scan on startup only */
      THIS_KIF->scan_time = (yyvsp[0].i);
   }
#line 4315 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1004 "cf-parse.y" /* yacc.c:1646  */
    {
     struct kif_primary_item *kpi = cfg_alloc(sizeof (struct kif_primary_item));
     kpi->pattern = (yyvsp[-1].t);
     kpi->prefix = (yyvsp[0].px).addr;
     kpi->pxlen = (yyvsp[0].px).len;
     add_tail(&THIS_KIF->primary, &kpi->n);
   }
#line 4327 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1018 "cf-parse.y" /* yacc.c:1646  */
    {
	if ((yyvsp[0].i) <= 0 || (yyvsp[0].i) >= NL_NUM_TABLES)
	  cf_error("Kernel routing table number out of range");
	THIS_KRT->sys.table_id = (yyvsp[0].i);
   }
#line 4337 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1036 "cf-parse.y" /* yacc.c:1646  */
    { new_config->router_id = (yyvsp[-1].i32); }
#line 4343 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1037 "cf-parse.y" /* yacc.c:1646  */
    { new_config->router_id_from = this_ipatt; }
#line 4349 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1041 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (yyvsp[0].i); }
#line 4355 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1042 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = f_eval_int((yyvsp[-1].x)); }
#line 4361 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1044 "cf-parse.y" /* yacc.c:1646  */
    {
#ifndef IPV6
     (yyval.i32) = ipa_to_u32((yyvsp[0].a));
#else
     cf_error("Router IDs must be entered as hexadecimal numbers or IPv4 addresses in IPv6 version");
#endif
   }
#line 4373 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1051 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class == (SYM_CONSTANT | T_INT) || (yyvsp[0].s)->class == (SYM_CONSTANT | T_QUAD))
       (yyval.i32) = SYM_VAL((yyvsp[0].s)).i;
#ifndef IPV6
     else if ((yyvsp[0].s)->class == (SYM_CONSTANT | T_IP))
       (yyval.i32) = ipa_to_u32(SYM_VAL((yyvsp[0].s)).px.ip);
#endif
     else
       cf_error("Number of IPv4 address constant expected");
   }
#line 4388 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1073 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_addr = (yyvsp[0].a); }
#line 4394 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1074 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_port = (yyvsp[0].i); }
#line 4400 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1075 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_flags = 0; }
#line 4406 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1076 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_flags = 1; }
#line 4412 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1081 "cf-parse.y" /* yacc.c:1646  */
    { new_config->gr_wait = (yyvsp[-1].i); }
#line 4418 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1087 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4424 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1088 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 4430 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1092 "cf-parse.y" /* yacc.c:1646  */
    {
   struct rtable_config *cf;
   cf = rt_new_table((yyvsp[-1].s));
   cf->sorted = (yyvsp[0].i);
   }
#line 4440 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1100 "cf-parse.y" /* yacc.c:1646  */
    {
  this_roa_table = roa_new_table_config((yyvsp[0].s));
}
#line 4448 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1106 "cf-parse.y" /* yacc.c:1646  */
    {
     roa_add_item_config(this_roa_table, (yyvsp[-5].px).addr, (yyvsp[-5].px).len, (yyvsp[-3].i), (yyvsp[-1].i));
   }
#line 4456 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1120 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = SYM_PROTO; }
#line 4462 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1121 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = SYM_TEMPLATE; }
#line 4468 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1125 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *s = cf_default_name(this_proto->protocol->template, &this_proto->protocol->name_counter);
     s->class = this_proto->class;
     s->def = this_proto;
     this_proto->name = s->name;
     }
#line 4479 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1131 "cf-parse.y" /* yacc.c:1646  */
    {
     cf_define_symbol((yyvsp[0].s), this_proto->class, this_proto);
     this_proto->name = (yyvsp[0].s)->name;
   }
#line 4488 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1135 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *s = cf_default_name(this_proto->protocol->template, &this_proto->protocol->name_counter);
     s->class = this_proto->class;
     s->def = this_proto;
     this_proto->name = s->name;

     if (((yyvsp[0].s)->class != SYM_TEMPLATE) && ((yyvsp[0].s)->class != SYM_PROTO)) cf_error("Template or protocol name expected");
     proto_copy_config(this_proto, (yyvsp[0].s)->def);
   }
#line 4502 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1144 "cf-parse.y" /* yacc.c:1646  */
    {
     cf_define_symbol((yyvsp[-2].s), this_proto->class, this_proto);
     this_proto->name = (yyvsp[-2].s)->name;

     if (((yyvsp[0].s)->class != SYM_TEMPLATE) && ((yyvsp[0].s)->class != SYM_PROTO)) cf_error("Template or protocol name expected");
     proto_copy_config(this_proto, (yyvsp[0].s)->def);
   }
#line 4514 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1155 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].i) < 0 || (yyvsp[0].i) > 0xFFFF) cf_error("Invalid preference");
     this_proto->preference = (yyvsp[0].i);
   }
#line 4523 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1159 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->disabled = (yyvsp[0].i); }
#line 4529 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1160 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->debug = (yyvsp[0].i); }
#line 4535 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1161 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->mrtdump = (yyvsp[0].i); }
#line 4541 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1162 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->in_filter = (yyvsp[0].f); }
#line 4547 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1163 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->out_filter = (yyvsp[0].f); }
#line 4553 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1164 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->rx_limit = (yyvsp[0].g); }
#line 4559 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1165 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->in_limit = (yyvsp[0].g); }
#line 4565 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1166 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->out_limit = (yyvsp[0].g); }
#line 4571 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1167 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->in_keep_filtered = (yyvsp[0].i); }
#line 4577 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1168 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->table = (yyvsp[0].r); }
#line 4583 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1169 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->router_id = (yyvsp[0].i32); }
#line 4589 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1170 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->dsc = (yyvsp[0].t); }
#line 4595 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1174 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.f) = (yyvsp[0].f); }
#line 4601 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1176 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.f) = FILTER_ACCEPT; }
#line 4607 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1177 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.f) = FILTER_REJECT; }
#line 4613 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1181 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_DISABLE; }
#line 4619 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1182 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_WARN; }
#line 4625 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 1183 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_BLOCK; }
#line 4631 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1184 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_RESTART; }
#line 4637 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 1185 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_DISABLE; }
#line 4643 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1189 "cf-parse.y" /* yacc.c:1646  */
    {
     struct proto_limit *l = cfg_allocz(sizeof(struct proto_limit));
     l->limit = (yyvsp[-1].i);
     l->action = (yyvsp[0].i);
     (yyval.g) = l;
   }
#line 4654 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1195 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.g) = NULL; }
#line 4660 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1199 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != SYM_TABLE) cf_error("Table name expected");
     (yyval.r) = (yyvsp[0].s)->def;
   }
#line 4669 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1207 "cf-parse.y" /* yacc.c:1646  */
    { new_config->proto_default_debug = (yyvsp[0].i); }
#line 4675 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 1208 "cf-parse.y" /* yacc.c:1646  */
    { new_config->cli_debug = (yyvsp[0].i); }
#line 4681 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 1216 "cf-parse.y" /* yacc.c:1646  */
    {
     struct iface_patt_node *ipn = cfg_allocz(sizeof(struct iface_patt_node));
     add_tail(&this_ipatt->ipn_list, NODE ipn);
     this_ipn = ipn;
   }
#line 4691 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 1224 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->pattern = (yyvsp[0].t); this_ipn->prefix = IPA_NONE; this_ipn->pxlen = 0; }
#line 4697 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 1225 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->pattern = NULL; this_ipn->prefix = (yyvsp[0].px).addr; this_ipn->pxlen = (yyvsp[0].px).len; }
#line 4703 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 1226 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->pattern = (yyvsp[-1].t); this_ipn->prefix = (yyvsp[0].px).addr; this_ipn->pxlen = (yyvsp[0].px).len; }
#line 4709 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 1230 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->positive = 1; }
#line 4715 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 1231 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->positive = 0; }
#line 4721 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 1245 "cf-parse.y" /* yacc.c:1646  */
    { iface_patt_check(); }
#line 4727 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 1247 "cf-parse.y" /* yacc.c:1646  */
    {
   /* Generic this_ipatt init */
   this_ipatt = cfg_allocz(sizeof(struct iface_patt));
   init_list(&this_ipatt->ipn_list);
 }
#line 4737 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 1259 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i) & 0xfc;        if (((yyvsp[0].i) < 0) || ((yyvsp[0].i) > 255)) cf_error("TX class must be in range 0-255"); }
#line 4743 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 1260 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ((yyvsp[0].i) & 0x3f) << 2; if (((yyvsp[0].i) < 0) || ((yyvsp[0].i) > 63))  cf_error("TX DSCP must be in range 0-63"); }
#line 4749 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 1266 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_device, (yyvsp[-1].i));
     init_list(&DIRECT_CFG->iface_list);
   }
#line 4758 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 1279 "cf-parse.y" /* yacc.c:1646  */
    {
     this_ipatt = cfg_allocz(sizeof(struct iface_patt));
     add_tail(&DIRECT_CFG->iface_list, NODE this_ipatt);
     init_list(&this_ipatt->ipn_list);
   }
#line 4768 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 1293 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 4774 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 1294 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4780 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 1295 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 4786 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 1300 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 4792 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 1304 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_STATES; }
#line 4798 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 1305 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_ROUTES; }
#line 4804 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 1306 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_FILTERS; }
#line 4810 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 1307 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_IFACES; }
#line 4816 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 1308 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_EVENTS; }
#line 4822 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 1309 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_PACKETS; }
#line 4828 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 1315 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 4834 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 1316 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4840 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1317 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 4846 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 1322 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 4852 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 1326 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = MD_STATES; }
#line 4858 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 1327 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = MD_MESSAGES; }
#line 4864 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 1348 "cf-parse.y" /* yacc.c:1646  */
    {
     if (!this_p_list) {
     	this_p_list = cfg_alloc(sizeof(list));
     	init_list(this_p_list);
        password_id = 1;
     }
     this_p_item = cfg_alloc(sizeof (struct password_item));
     this_p_item->password = (yyvsp[0].t);
     this_p_item->genfrom = 0;
     this_p_item->gento = TIME_INFINITY;
     this_p_item->accfrom = 0;
     this_p_item->accto = TIME_INFINITY;
     this_p_item->id = password_id++;
     add_tail(this_p_list, &this_p_item->n);
   }
#line 4884 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 1366 "cf-parse.y" /* yacc.c:1646  */
    { }
#line 4890 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 1367 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->genfrom = (yyvsp[-2].time); }
#line 4896 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1368 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->gento = (yyvsp[-2].time); }
#line 4902 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 1369 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->accfrom = (yyvsp[-2].time); }
#line 4908 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 1370 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->accto = (yyvsp[-2].time); }
#line 4914 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 1371 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->id = (yyvsp[-2].i); if ((yyvsp[-2].i) <= 0) cf_error("Password ID has to be greated than zero."); }
#line 4920 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 1380 "cf-parse.y" /* yacc.c:1646  */
    { cmd_show_status(); }
#line 4926 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1383 "cf-parse.y" /* yacc.c:1646  */
    { cmd_show_memory(); }
#line 4932 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1386 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_show, 0, 0); }
#line 4938 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 1389 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_show, 0, 1); }
#line 4944 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1393 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s) = NULL; }
#line 4950 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1397 "cf-parse.y" /* yacc.c:1646  */
    { if_show(); }
#line 4956 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1400 "cf-parse.y" /* yacc.c:1646  */
    { if_show_summary(); }
#line 4962 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1404 "cf-parse.y" /* yacc.c:1646  */
    { rt_show((yyvsp[-1].ra)); }
#line 4968 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 1407 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = cfg_allocz(sizeof(struct rt_show_data));
     (yyval.ra)->pxlen = 256;
     (yyval.ra)->filter = FILTER_ACCEPT;
   }
#line 4978 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1412 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     if ((yyval.ra)->pxlen != 256) cf_error("Only one prefix expected");
     (yyval.ra)->prefix = (yyvsp[0].px).addr;
     (yyval.ra)->pxlen = (yyvsp[0].px).len;
   }
#line 4989 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1418 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->pxlen != 256) cf_error("Only one prefix expected");
     (yyval.ra)->prefix = (yyvsp[0].px).addr;
     (yyval.ra)->pxlen = (yyvsp[0].px).len;
     (yyval.ra)->show_for = 1;
   }
#line 5001 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 1425 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyvsp[0].s)->class != SYM_TABLE) cf_error("%s is not a table", (yyvsp[0].s)->name);
     (yyval.ra)->table = ((struct rtable_config *)(yyvsp[0].s)->def)->table;
   }
#line 5011 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1430 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->filter != FILTER_ACCEPT) cf_error("Filter specified twice");
     (yyval.ra)->filter = (yyvsp[0].f);
   }
#line 5021 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1435 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     if ((yyval.ra)->filter != FILTER_ACCEPT) cf_error("Filter specified twice");
     (yyval.ra)->filter = (yyvsp[0].f);
   }
#line 5031 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1440 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->verbose = 1;
   }
#line 5040 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1444 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->primary_only = 1;
   }
#line 5049 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1448 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->filtered = 1;
   }
#line 5058 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1452 "cf-parse.y" /* yacc.c:1646  */
    {
     struct proto_config *c = (struct proto_config *) (yyvsp[0].s)->def;
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->export_mode) cf_error("Protocol specified twice");
     if ((yyvsp[0].s)->class != SYM_PROTO || !c->proto) cf_error("%s is not a protocol", (yyvsp[0].s)->name);
     (yyval.ra)->export_mode = (yyvsp[-1].i);
     (yyval.ra)->export_protocol = c->proto;
     (yyval.ra)->running_on_config = c->proto->cf->global;
   }
#line 5072 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1461 "cf-parse.y" /* yacc.c:1646  */
    {
     struct proto_config *c = (struct proto_config *) (yyvsp[0].s)->def;
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->show_protocol) cf_error("Protocol specified twice");
     if ((yyvsp[0].s)->class != SYM_PROTO || !c->proto) cf_error("%s is not a protocol", (yyvsp[0].s)->name);
     (yyval.ra)->show_protocol = c->proto;
     (yyval.ra)->running_on_config = c->proto->cf->global;
   }
#line 5085 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1469 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->stats = 1;
   }
#line 5094 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1473 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->stats = 2;
   }
#line 5103 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1480 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = RSEM_PREEXPORT; }
#line 5109 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1481 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = RSEM_EXPORT; }
#line 5115 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1482 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = RSEM_NOEXPORT; }
#line 5121 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1488 "cf-parse.y" /* yacc.c:1646  */
    { roa_show((yyvsp[-1].ro)); }
#line 5127 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1491 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = cfg_allocz(sizeof(struct roa_show_data));
     (yyval.ro)->mode = ROA_SHOW_ALL;
     (yyval.ro)->table = roa_table_default;
     if (roa_table_default == NULL)
       cf_error("No ROA table defined");
   }
#line 5139 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1498 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = (yyvsp[-2].ro);
     if ((yyval.ro)->mode != ROA_SHOW_ALL) cf_error("Only one prefix expected");
     (yyval.ro)->prefix = (yyvsp[0].px).addr;
     (yyval.ro)->pxlen = (yyvsp[0].px).len;
     (yyval.ro)->mode = (yyvsp[-1].i);
   }
#line 5151 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1505 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = (yyvsp[-2].ro);
     (yyval.ro)->asn = (yyvsp[0].i);
   }
#line 5160 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1509 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = (yyvsp[-2].ro);
     if ((yyvsp[0].s)->class != SYM_ROA) cf_error("%s is not a ROA table", (yyvsp[0].s)->name);
     (yyval.ro)->table = ((struct roa_table_config *)(yyvsp[0].s)->def)->table;
   }
#line 5170 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1517 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROA_SHOW_PX; }
#line 5176 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1518 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROA_SHOW_IN; }
#line 5182 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1519 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROA_SHOW_FOR; }
#line 5188 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1525 "cf-parse.y" /* yacc.c:1646  */
    { cmd_show_symbols((yyvsp[-1].sd)); }
#line 5194 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1528 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.sd) = cfg_allocz(sizeof(struct sym_show_data));
   }
#line 5202 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1531 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_TABLE; }
#line 5208 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1532 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_FUNCTION; }
#line 5214 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1533 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_FILTER; }
#line 5220 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1534 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_ROLE_MAP; }
#line 5226 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1535 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_PROTO; }
#line 5232 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1536 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_TEMPLATE; }
#line 5238 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1537 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_ROA; }
#line 5244 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1538 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->sym = (yyvsp[0].s); }
#line 5250 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1543 "cf-parse.y" /* yacc.c:1646  */
    { 
     if (roa_table_default == NULL)
       cf_error("No ROA table defined");
     (yyval.rot) = roa_table_default;
   }
#line 5260 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1548 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != SYM_ROA)
       cf_error("%s is not a ROA table", (yyvsp[0].s)->name);
     (yyval.rot) = ((struct roa_table_config *)(yyvsp[0].s)->def)->table;
   }
#line 5270 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1557 "cf-parse.y" /* yacc.c:1646  */
    {
  if (! cli_access_restricted())
    { roa_add_item((yyvsp[-1].rot), (yyvsp[-6].px).addr, (yyvsp[-6].px).len, (yyvsp[-4].i), (yyvsp[-2].i), ROA_SRC_DYNAMIC); cli_msg(0, ""); }
}
#line 5279 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1564 "cf-parse.y" /* yacc.c:1646  */
    {
  if (! cli_access_restricted())
    { roa_delete_item((yyvsp[-1].rot), (yyvsp[-6].px).addr, (yyvsp[-6].px).len, (yyvsp[-4].i), (yyvsp[-2].i), ROA_SRC_DYNAMIC); cli_msg(0, ""); }
}
#line 5288 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1571 "cf-parse.y" /* yacc.c:1646  */
    {
  if (! cli_access_restricted())
    { roa_flush((yyvsp[-1].rot), ROA_SRC_DYNAMIC); cli_msg(0, ""); }
}
#line 5297 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1579 "cf-parse.y" /* yacc.c:1646  */
    { rdump(&root_pool); cli_msg(0, ""); }
#line 5303 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1581 "cf-parse.y" /* yacc.c:1646  */
    { sk_dump_all(); cli_msg(0, ""); }
#line 5309 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1583 "cf-parse.y" /* yacc.c:1646  */
    { io_log_dump(); cli_msg(0, ""); }
#line 5315 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1585 "cf-parse.y" /* yacc.c:1646  */
    { if_dump_all(); cli_msg(0, ""); }
#line 5321 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1587 "cf-parse.y" /* yacc.c:1646  */
    { neigh_dump_all(); cli_msg(0, ""); }
#line 5327 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1589 "cf-parse.y" /* yacc.c:1646  */
    { rta_dump_all(); cli_msg(0, ""); }
#line 5333 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1591 "cf-parse.y" /* yacc.c:1646  */
    { rt_dump_all(); cli_msg(0, ""); }
#line 5339 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1593 "cf-parse.y" /* yacc.c:1646  */
    { protos_dump_all(); cli_msg(0, ""); }
#line 5345 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1596 "cf-parse.y" /* yacc.c:1646  */
    { cmd_eval((yyvsp[-1].x)); }
#line 5351 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1599 "cf-parse.y" /* yacc.c:1646  */
    {
  cli_set_log_echo(this_cli, (yyvsp[-2].i), (yyvsp[-1].i));
  cli_msg(0, "");
}
#line 5360 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1605 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 5366 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1606 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 5372 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1607 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 5378 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1611 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 4096; }
#line 5384 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1612 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].i) < 256 || (yyvsp[0].i) > 65536) cf_error("Invalid log buffer size");
     (yyval.i) = (yyvsp[0].i);
   }
#line 5393 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1619 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_disable, 1, 0); }
#line 5399 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1621 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_enable, 1, 0); }
#line 5405 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1623 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_restart, 1, 0); }
#line 5411 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1625 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_reload, 1, CMD_RELOAD); }
#line 5417 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1627 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_reload, 1, CMD_RELOAD_IN); }
#line 5423 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1629 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_reload, 1, CMD_RELOAD_OUT); }
#line 5429 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1633 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-2].ps), proto_cmd_debug, 1, (yyvsp[-1].i)); }
#line 5435 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1637 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-2].ps), proto_cmd_mrtdump, 1, (yyvsp[-1].i)); }
#line 5441 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1640 "cf-parse.y" /* yacc.c:1646  */
    { this_cli->restricted = 1; cli_msg(16, "Access restricted"); }
#line 5447 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1643 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].s); (yyval.ps).patt = 0; }
#line 5453 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1644 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = NULL; (yyval.ps).patt = 1; }
#line 5459 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1645 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].t); (yyval.ps).patt = 1; }
#line 5465 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1649 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].s); (yyval.ps).patt = 0; }
#line 5471 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1650 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = NULL; (yyval.ps).patt = 1; }
#line 5477 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1651 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].t); (yyval.ps).patt = 1; }
#line 5483 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1659 "cf-parse.y" /* yacc.c:1646  */
    { (yyvsp[0].s) = cf_define_symbol((yyvsp[0].s), SYM_FILTER, NULL); cf_push_scope( (yyvsp[0].s) ); }
#line 5489 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1660 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyvsp[-2].s)->def = (yyvsp[0].f);
     (yyvsp[0].f)->name = (yyvsp[-2].s)->name;
     DBG( "We have new filter defined (%s)\n", (yyvsp[-2].s)->name );
     cf_pop_scope();
   }
#line 5500 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1669 "cf-parse.y" /* yacc.c:1646  */
    { (yyvsp[0].s) = cf_define_symbol((yyvsp[0].s), SYM_ROLE_MAP, NULL); cf_push_scope( (yyvsp[0].s) ); }
#line 5506 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1670 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyvsp[-2].s)->def = (yyvsp[0].rm);
     (yyvsp[0].rm)->name = (yyvsp[-2].s)->name;
     DBG( "We have new role_map defined (%s)\n", (yyvsp[-2].s)->name );
     cf_pop_scope();
   }
#line 5517 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1679 "cf-parse.y" /* yacc.c:1646  */
    { f_eval_int((yyvsp[0].x)); }
#line 5523 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1683 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_INT; }
#line 5529 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1684 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_BOOL; }
#line 5535 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1685 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_IP; }
#line 5541 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1686 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PREFIX; }
#line 5547 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1687 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PAIR; }
#line 5553 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1688 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_QUAD; }
#line 5559 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1689 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_EC; }
#line 5565 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1690 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_STRING; }
#line 5571 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1691 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PATH_MASK; }
#line 5577 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1692 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PATH; }
#line 5583 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1693 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_CLIST; }
#line 5589 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1694 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_ECLIST; }
#line 5595 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1695 "cf-parse.y" /* yacc.c:1646  */
    { 
	switch ((yyvsp[-1].i)) {
	  case T_INT:
	  case T_PAIR:
	  case T_QUAD:
	  case T_EC:
	  case T_IP:
	       (yyval.i) = T_SET;
	       break;

	  case T_PREFIX:
	       (yyval.i) = T_PREFIX_SET;
	    break;

	  default:
		cf_error( "You can't create sets of this type." );
	}
   }
#line 5618 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1716 "cf-parse.y" /* yacc.c:1646  */
    {
     struct f_val * val = cfg_alloc(sizeof(struct f_val));
     val->type = T_VOID;
     (yyvsp[0].s) = cf_define_symbol((yyvsp[0].s), SYM_VARIABLE | (yyvsp[-1].i), val);
     DBG( "New variable %s type %x\n", (yyvsp[0].s)->name, (yyvsp[-1].i) );
     (yyvsp[0].s)->aux2 = NULL;
     (yyval.s)=(yyvsp[0].s);
   }
#line 5631 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1727 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s) = NULL; }
#line 5637 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1728 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.s) = (yyvsp[-2].s);
     (yyval.s)->aux2 = (yyvsp[0].s);
   }
#line 5646 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1735 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5652 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1736 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.s) = (yyvsp[-2].s);
     (yyval.s)->aux2 = (yyvsp[0].s);
   }
#line 5661 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1743 "cf-parse.y" /* yacc.c:1646  */
    {
     struct filter *f = cfg_alloc(sizeof(struct filter));
     f->name = NULL;
     f->root = (yyvsp[0].x);
     (yyval.f) = f;
   }
#line 5672 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1752 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROLE_PEER; }
#line 5678 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1753 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROLE_PROV; }
#line 5684 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1754 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROLE_CUST; }
#line 5690 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1755 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROLE_INTE; }
#line 5696 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1759 "cf-parse.y" /* yacc.c:1646  */
    {
     struct role_map *rm = cfg_alloc(sizeof(struct role_map));
     rm->name = NULL;
     struct f_inst *default_role = f_new_inst();
     default_role->code = 'c';
     default_role->aux = T_ROLE;
     default_role->a2.i = ROLE_UNKN;
     struct f_inst *tmp = (yyvsp[0].x);
     while (tmp->next) tmp = tmp->next;
     tmp->next = default_role;
     struct f_inst *strip_result = f_new_inst();
     strip_result->code = P('c','a');
     strip_result->a1.p = NULL;
     strip_result->a2.p = (yyvsp[0].x);
     rm->root = strip_result;
     (yyval.rm) = rm;
   }
#line 5718 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1779 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != SYM_FILTER) cf_error("No such filter.");
     (yyval.f) = (yyvsp[0].s)->def;
   }
#line 5727 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1787 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != SYM_ROLE_MAP) cf_error("No such role_map.");
     (yyval.rm) = (yyvsp[0].s)->def;
   }
#line 5736 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1795 "cf-parse.y" /* yacc.c:1646  */
    {
     /* Construct 'IF term THEN ACCEPT; REJECT;' */
     struct filter *f = cfg_alloc(sizeof(struct filter));
     struct f_inst *i, *acc, *rej;
     acc = f_new_inst();		/* ACCEPT */
     acc->code = P('p',',');
     acc->a1.p = NULL;
     acc->a2.i = F_ACCEPT;
     rej = f_new_inst();		/* REJECT */
     rej->code = P('p',',');
     rej->a1.p = NULL;
     rej->a2.i = F_REJECT;
     i = f_new_inst();			/* IF */
     i->code = '?';
     i->a1.p = (yyvsp[0].x);
     i->a2.p = acc;
     i->next = rej;
     f->name = NULL;
     f->root = i;
     (yyval.f) = f;
  }
#line 5762 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1819 "cf-parse.y" /* yacc.c:1646  */
    { DBG( "Have function parameters\n" ); (yyval.s)=(yyvsp[-1].s); }
#line 5768 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1820 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s)=NULL; }
#line 5774 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1824 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[-3].s)) {
       /* Prepend instruction to clear local variables */
       (yyval.x) = f_new_inst();
       (yyval.x)->code = P('c','v');
       (yyval.x)->a1.p = (yyvsp[-3].s);
       (yyval.x)->next = (yyvsp[-1].x);
     } else
       (yyval.x) = (yyvsp[-1].x);
   }
#line 5789 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1837 "cf-parse.y" /* yacc.c:1646  */
    { DBG( "Beginning of function %s\n", (yyvsp[0].s)->name );
     (yyvsp[0].s) = cf_define_symbol((yyvsp[0].s), SYM_FUNCTION, NULL);
     cf_push_scope((yyvsp[0].s));
   }
#line 5798 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1840 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyvsp[-3].s)->def = (yyvsp[0].x);
     (yyvsp[-3].s)->aux2 = (yyvsp[-1].s);
     DBG("Hmm, we've got one function here - %s\n", (yyvsp[-3].s)->name); 
     cf_pop_scope();
   }
#line 5809 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1853 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 5815 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1854 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x)->next; (yyvsp[0].x)->next = NULL; }
#line 5821 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1857 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyvsp[0].x)->next = (yyvsp[0].x); }
#line 5827 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1858 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyvsp[0].x)->next = (yyvsp[-1].x)->next ; (yyvsp[-1].x)->next = (yyvsp[0].x); }
#line 5833 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1862 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x)=(yyvsp[0].x);
   }
#line 5841 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1865 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x)=(yyvsp[-1].x);
   }
#line 5849 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1874 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_IP; (yyval.v).val.px.ip = (yyvsp[0].a); }
#line 5855 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1887 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_INT; (yyval.v).val.i = (yyvsp[0].i); }
#line 5861 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1888 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_QUAD; (yyval.v).val.i = (yyvsp[0].i32); }
#line 5867 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1889 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 5873 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1890 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = pair_a((yyvsp[0].i)); (yyval.v).val.i = pair_b((yyvsp[0].i)); }
#line 5879 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1891 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.v) = f_eval((yyvsp[-1].x), cfg_mem);
     if (!f_valid_set_type((yyval.v).type)) cf_error("Set-incompatible type");
   }
#line 5888 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1895 "cf-parse.y" /* yacc.c:1646  */
    {
     if (!cf_symbol_is_constant((yyvsp[0].s))) cf_error("%s: constant expected", (yyvsp[0].s)->name);
     if (!f_valid_set_type(SYM_TYPE((yyvsp[0].s)))) cf_error("%s: set-incompatible type", (yyvsp[0].s)->name);
     (yyval.v) = *(struct f_val *)((yyvsp[0].s)->def);
   }
#line 5898 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1903 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_INT; (yyval.v).val.i = (yyvsp[0].i); }
#line 5904 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1904 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_INT; (yyval.v).val.i = f_eval_int((yyvsp[-1].x)); }
#line 5910 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1905 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_QUAD; (yyval.v).val.i = (yyvsp[0].i32); }
#line 5916 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1906 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 5922 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1907 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = pair_a((yyvsp[0].i)); (yyval.v).val.i = pair_b((yyvsp[0].i)); }
#line 5928 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1911 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = f_eval_int((yyvsp[0].x)); check_u16((yyval.i)); }
#line 5934 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1914 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = pair((yyvsp[0].i), (yyvsp[0].i)); }
#line 5940 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1915 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = pair((yyvsp[-2].i), (yyvsp[0].i)); }
#line 5946 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1916 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = 0xFFFF; }
#line 5952 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1920 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.e) = f_new_pair_set(pair_a((yyvsp[-3].i32)), pair_b((yyvsp[-3].i32)), pair_a((yyvsp[-1].i32)), pair_b((yyvsp[-1].i32)));
   }
#line 5960 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1923 "cf-parse.y" /* yacc.c:1646  */
    {
     /* Hack: $2 and $4 should be pair_expr, but that would cause shift/reduce conflict */
     if ((pair_a((yyvsp[-9].i32)) != pair_b((yyvsp[-9].i32))) || (pair_a((yyvsp[-7].i32)) != pair_b((yyvsp[-7].i32))))
       cf_error("syntax error");
     (yyval.e) = f_new_pair_item(pair_b((yyvsp[-9].i32)), (yyvsp[-3].i), pair_b((yyvsp[-7].i32)), (yyvsp[-1].i)); 
   }
#line 5971 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1932 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = f_eval_int((yyvsp[0].x)); }
#line 5977 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1935 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = EC_RT; }
#line 5983 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1936 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = EC_RO; }
#line 5989 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1937 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 5995 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1938 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = EC_GENERIC; }
#line 6001 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1942 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_ec_item((yyvsp[-5].i), 0, (yyvsp[-3].i32), (yyvsp[-1].i32), (yyvsp[-1].i32)); }
#line 6007 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1943 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_ec_item((yyvsp[-7].i), 0, (yyvsp[-5].i32), (yyvsp[-3].i32), (yyvsp[-1].i32)); }
#line 6013 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1944 "cf-parse.y" /* yacc.c:1646  */
    {  (yyval.e) = f_new_ec_item((yyvsp[-5].i), 0, (yyvsp[-3].i32), 0, EC_ALL); }
#line 6019 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1950 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[0].v), (yyvsp[0].v)); }
#line 6025 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1951 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[-2].v), (yyvsp[0].v)); }
#line 6031 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1957 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[0].v), (yyvsp[0].v)); }
#line 6037 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1958 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[-2].v), (yyvsp[0].v)); }
#line 6043 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1963 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_merge_items((yyvsp[-2].e), (yyvsp[0].e)); }
#line 6049 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1968 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_merge_items((yyvsp[-2].e), (yyvsp[0].e)); }
#line 6055 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1972 "cf-parse.y" /* yacc.c:1646  */
    {
     if (((yyvsp[0].i) < 0) || ((yyvsp[0].i) > MAX_PREFIX_LENGTH) || !ip_is_prefix((yyvsp[-2].a), (yyvsp[0].i))) cf_error("Invalid network prefix: %I/%d.", (yyvsp[-2].a), (yyvsp[0].i));
     (yyval.v).type = T_PREFIX; (yyval.v).val.px.ip = (yyvsp[-2].a); (yyval.v).val.px.len = (yyvsp[0].i);
   }
#line 6064 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1979 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 6070 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1980 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[-1].v); (yyval.v).val.px.len |= LEN_PLUS; }
#line 6076 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1981 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[-1].v); (yyval.v).val.px.len |= LEN_MINUS; }
#line 6082 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1982 "cf-parse.y" /* yacc.c:1646  */
    { 
     if (! ((0 <= (yyvsp[-3].i)) && ((yyvsp[-3].i) <= (yyvsp[-1].i)) && ((yyvsp[-1].i) <= MAX_PREFIX_LENGTH))) cf_error("Invalid prefix pattern range: {%d, %d}.", (yyvsp[-3].i), (yyvsp[-1].i));
     (yyval.v) = (yyvsp[-5].v); (yyval.v).val.px.len |= LEN_RANGE | ((yyvsp[-3].i) << 16) | ((yyvsp[-1].i) << 8);
   }
#line 6091 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1989 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.trie) = f_new_trie(cfg_mem, sizeof(struct f_trie_node)); trie_add_fprefix((yyval.trie), &((yyvsp[0].v).val.px)); }
#line 6097 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1990 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.trie) = (yyvsp[-2].trie); trie_add_fprefix((yyval.trie), &((yyvsp[0].v).val.px)); }
#line 6103 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1993 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = NULL; }
#line 6109 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1994 "cf-parse.y" /* yacc.c:1646  */
    {
     /* Fill data fields */
     struct f_tree *t;
     for (t = (yyvsp[-2].e); t; t = t->left)
       t->data = (yyvsp[0].x);
     (yyval.e) = f_merge_items((yyvsp[-3].e), (yyvsp[-2].e));
   }
#line 6121 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 2001 "cf-parse.y" /* yacc.c:1646  */
    { 
     struct f_tree *t = f_new_tree();
     t->from.type = t->to.type = T_VOID;
     t->right = t;
     t->data = (yyvsp[0].x);
     (yyval.e) = f_merge_items((yyvsp[-2].e), t);
 }
#line 6133 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 2013 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6139 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 2014 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[-1].x); }
#line 6145 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 2018 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = (yyvsp[-1].h); }
#line 6151 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 2019 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = (yyvsp[-1].h); }
#line 6157 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 2023 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASN;      (yyval.h)->val = (yyvsp[-1].i); }
#line 6163 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 2024 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASTERISK; (yyval.h)->val  = 0; }
#line 6169 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 377:
#line 2025 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_QUESTION; (yyval.h)->val  = 0; }
#line 6175 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 378:
#line 2026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASN_EXPR; (yyval.h)->val = (uintptr_t) (yyvsp[-1].x); }
#line 6181 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 2027 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = NULL; }
#line 6187 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 2031 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASN;      (yyval.h)->val = (yyvsp[-1].i); }
#line 6193 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 2032 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASTERISK; (yyval.h)->val  = 0; }
#line 6199 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 382:
#line 2033 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = NULL; }
#line 6205 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 2037 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_INT;  (yyval.x)->a2.i = (yyvsp[0].i); }
#line 6211 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 2038 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_BOOL; (yyval.x)->a2.i = 1;  }
#line 6217 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 385:
#line 2039 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_BOOL; (yyval.x)->a2.i = 0;  }
#line 6223 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 386:
#line 2040 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_STRING; (yyval.x)->a2.p = (yyvsp[0].t); }
#line 6229 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 387:
#line 2041 "cf-parse.y" /* yacc.c:1646  */
    { NEW_F_VAL; (yyval.x) = f_new_inst(); (yyval.x)->code = 'C'; (yyval.x)->a1.p = val; *val = (yyvsp[0].v); }
#line 6235 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 388:
#line 2042 "cf-parse.y" /* yacc.c:1646  */
    {NEW_F_VAL; (yyval.x) = f_new_inst(); (yyval.x)->code = 'C'; (yyval.x)->a1.p = val; *val = (yyvsp[0].v); }
#line 6241 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 389:
#line 2043 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_QUAD;  (yyval.x)->a2.i = (yyvsp[0].i32); }
#line 6247 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 390:
#line 2044 "cf-parse.y" /* yacc.c:1646  */
    { DBG( "We've got a set here..." ); (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_SET; (yyval.x)->a2.p = build_tree((yyvsp[-1].e)); DBG( "ook\n" ); }
#line 6253 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 391:
#line 2045 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_PREFIX_SET;  (yyval.x)->a2.p = (yyvsp[-1].trie); }
#line 6259 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 392:
#line 2046 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = (yyvsp[0].i) >> 16; (yyval.x)->a2.i = (yyvsp[0].i) & 0xffff; }
#line 6265 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 393:
#line 2047 "cf-parse.y" /* yacc.c:1646  */
    { NEW_F_VAL; (yyval.x) = f_new_inst(); (yyval.x)->code = 'C'; val->type = T_PATH_MASK; val->val.path_mask = (yyvsp[0].h); (yyval.x)->a1.p = val; }
#line 6271 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 394:
#line 2051 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_dpair((yyvsp[-3].x), (yyvsp[-1].x)); }
#line 6277 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 395:
#line 2052 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_ec((yyvsp[-5].i), (yyvsp[-3].x), (yyvsp[-1].x)); }
#line 6283 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 397:
#line 2066 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *sym;
     struct f_inst *inst = (yyvsp[-1].x);
     if ((yyvsp[-3].s)->class != SYM_FUNCTION)
       cf_error("You can't call something which is not a function. Really.");
     DBG("You are calling function %s\n", (yyvsp[-3].s)->name);
     (yyval.x) = f_new_inst();
     (yyval.x)->code = P('c','a');
     (yyval.x)->a1.p = inst;
     (yyval.x)->a2.p = (yyvsp[-3].s)->def;
     sym = (yyvsp[-3].s)->aux2;
     while (sym || inst) {
       if (!sym || !inst)
	 cf_error("Wrong number of arguments for function %s.", (yyvsp[-3].s)->name);
       DBG( "You should pass parameter called %s\n", sym->name);
       inst->a1.p = sym;
       sym = sym->aux2;
       inst = inst->next;
     }
   }
#line 6308 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 398:
#line 2089 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();

     switch ((yyvsp[0].s)->class & 0xff00) {
       case SYM_CONSTANT: (yyval.x)->code = 'C'; break;
       case SYM_VARIABLE: (yyval.x)->code = 'V'; break;
       default: cf_error("%s: variable expected.", (yyvsp[0].s)->name);
     }

     (yyval.x)->a1.p = (yyvsp[0].s)->def;
     (yyval.x)->a2.p = (yyvsp[0].s)->name;
   }
#line 6325 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 399:
#line 2103 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_IP;         (yyval.x)->a2.i = SA_FROM;	(yyval.x)->a1.i = 1; }
#line 6331 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 400:
#line 2104 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_IP;         (yyval.x)->a2.i = SA_GW;	(yyval.x)->a1.i = 1; }
#line 6337 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 401:
#line 2105 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_PREFIX;     (yyval.x)->a2.i = SA_NET; }
#line 6343 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 402:
#line 2106 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_STRING;     (yyval.x)->a2.i = SA_PROTO; }
#line 6349 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 403:
#line 2107 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_RTS;   (yyval.x)->a2.i = SA_SOURCE; }
#line 6355 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 404:
#line 2108 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_SCOPE; (yyval.x)->a2.i = SA_SCOPE;	(yyval.x)->a1.i = 1; }
#line 6361 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 405:
#line 2109 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_RTC;   (yyval.x)->a2.i = SA_CAST; }
#line 6367 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 406:
#line 2110 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_RTD;   (yyval.x)->a2.i = SA_DEST;	(yyval.x)->a1.i = 1; }
#line 6373 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 407:
#line 2111 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_STRING;     (yyval.x)->a2.i = SA_IFNAME; }
#line 6379 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 408:
#line 2112 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_INT;        (yyval.x)->a2.i = SA_IFINDEX; }
#line 6385 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 409:
#line 2116 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[-1].x); }
#line 6391 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 410:
#line 2117 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '+';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6397 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 411:
#line 2118 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '-';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6403 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 412:
#line 2119 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '*';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6409 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 413:
#line 2120 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '/';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6415 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 414:
#line 2121 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '&';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6421 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 415:
#line 2122 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '|';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6427 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 416:
#line 2123 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('=','='); (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6433 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 417:
#line 2124 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('!','=');     (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6439 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 418:
#line 2125 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '<';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6445 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 419:
#line 2126 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('<','=');     (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6451 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 420:
#line 2127 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '<';        (yyval.x)->a1.p = (yyvsp[0].x); (yyval.x)->a2.p = (yyvsp[-2].x); }
#line 6457 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 421:
#line 2128 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('<','=');     (yyval.x)->a1.p = (yyvsp[0].x); (yyval.x)->a2.p = (yyvsp[-2].x); }
#line 6463 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 422:
#line 2129 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '~';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6469 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 423:
#line 2130 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '!'; (yyval.x)->a1.p = (yyvsp[0].x); }
#line 6475 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 424:
#line 2131 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('d','e');  (yyval.x)->a1.p = (yyvsp[-1].x); }
#line 6481 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 425:
#line 2133 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6487 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 426:
#line 2134 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6493 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 427:
#line 2135 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6499 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 428:
#line 2137 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'P'; }
#line 6505 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 429:
#line 2139 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyval.x)->code = 'a'; }
#line 6511 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 430:
#line 2140 "cf-parse.y" /* yacc.c:1646  */
    {(yyval.x) = f_new_inst(); (yyval.x)->code = P('n','p'); }
#line 6517 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 431:
#line 2142 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyval.x)->code = P('e','a'); }
#line 6523 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 432:
#line 2144 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('c','p'); (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->aux = T_IP; }
#line 6529 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 433:
#line 2145 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'L'; (yyval.x)->a1.p = (yyvsp[-2].x); }
#line 6535 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 434:
#line 2146 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('i','M'); (yyval.x)->a1.p = (yyvsp[-5].x); (yyval.x)->a2.p = (yyvsp[-1].x); }
#line 6541 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 435:
#line 2147 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('a','f'); (yyval.x)->a1.p = (yyvsp[-2].x); }
#line 6547 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 436:
#line 2148 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('a','l'); (yyval.x)->a1.p = (yyvsp[-2].x); }
#line 6553 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 437:
#line 2158 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'E'; (yyval.x)->aux = T_PATH; }
#line 6559 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 438:
#line 2159 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'E'; (yyval.x)->aux = T_CLIST; }
#line 6565 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 439:
#line 2160 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'E'; (yyval.x)->aux = T_ECLIST; }
#line 6571 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 440:
#line 2161 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('A','p'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); }
#line 6577 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 441:
#line 2162 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('C','a'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); (yyval.x)->aux = 'a'; }
#line 6583 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 442:
#line 2163 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('C','a'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); (yyval.x)->aux = 'd'; }
#line 6589 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 443:
#line 2164 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('C','a'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); (yyval.x)->aux = 'f'; }
#line 6595 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 444:
#line 2166 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_roa_check((yyvsp[-1].s), NULL, NULL); }
#line 6601 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 445:
#line 2167 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_roa_check((yyvsp[-5].s), (yyvsp[-3].x), (yyvsp[-1].x)); }
#line 6607 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 446:
#line 2172 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *sym;
     struct f_inst *inst = (yyvsp[-1].x);
     if ((yyvsp[-3].s)->class != SYM_FUNCTION)
       cf_error("You can't call something which is not a function. Really.");
     DBG("You are calling function %s\n", (yyvsp[-3].s)->name);
     (yyval.x) = f_new_inst();
     (yyval.x)->code = P('c','a');
     (yyval.x)->a1.p = inst;
     (yyval.x)->a2.p = (yyvsp[-3].s)->def;
     sym = (yyvsp[-3].s)->aux2;
     while (sym || inst) {
       if (!sym || !inst)
	 cf_error("Wrong number of arguments for function %s.", (yyvsp[-3].s)->name);
       DBG( "You should pass parameter called %s\n", sym->name);
       inst->a1.p = sym;
       sym = sym->aux2;
       inst = inst->next;
     }
   }
#line 6632 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 447:
#line 2195 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_QUITBIRD; }
#line 6638 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 448:
#line 2196 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_ACCEPT; }
#line 6644 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 449:
#line 2197 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_REJECT; }
#line 6650 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 450:
#line 2198 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_ERROR; }
#line 6656 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 451:
#line 2199 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_NOP; }
#line 6662 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 452:
#line 2200 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_NONL; }
#line 6668 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 453:
#line 2204 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'p'; (yyval.x)->a1.p = (yyvsp[0].x); (yyval.x)->a2.p = NULL; }
#line 6674 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 454:
#line 2207 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 6680 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 455:
#line 2208 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6686 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 456:
#line 2209 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[-2].x)) {
       (yyvsp[-2].x)->next = (yyvsp[0].x);
       (yyval.x) = (yyvsp[-2].x);
     } else (yyval.x) = (yyvsp[0].x);
   }
#line 6697 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 457:
#line 2217 "cf-parse.y" /* yacc.c:1646  */
    { 
     (yyval.x) = f_new_inst();
     (yyval.x)->code = 's';
     (yyval.x)->a1.p = NULL;
     (yyval.x)->a2.p = (yyvsp[0].x);
     (yyval.x)->next = NULL;
   }
#line 6709 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 458:
#line 2224 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     (yyval.x)->code = 's';
     (yyval.x)->a1.p = NULL;
     (yyval.x)->a2.p = (yyvsp[-2].x);
     (yyval.x)->next = (yyvsp[0].x);
   }
#line 6721 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 459:
#line 2233 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 6727 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 460:
#line 2234 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6733 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 461:
#line 2238 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     (yyval.x)->code = '?';
     (yyval.x)->a1.p = (yyvsp[-2].x);
     (yyval.x)->a2.p = (yyvsp[0].x);
   }
#line 6744 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 462:
#line 2244 "cf-parse.y" /* yacc.c:1646  */
    {
     struct f_inst *i = f_new_inst();
     i->code = '?';
     i->a1.p = (yyvsp[-4].x);
     i->a2.p = (yyvsp[-2].x);
     (yyval.x) = f_new_inst();
     (yyval.x)->code = '?';
     (yyval.x)->a1.p = i;
     (yyval.x)->a2.p = (yyvsp[0].x);
   }
#line 6759 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 463:
#line 2254 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     DBG( "Ook, we'll set value\n" );
     if (((yyvsp[-3].s)->class & ~T_MASK) != SYM_VARIABLE)
       cf_error( "You may set only variables." );
     (yyval.x)->code = 's';
     (yyval.x)->a1.p = (yyvsp[-3].s);
     (yyval.x)->a2.p = (yyvsp[-1].x);
   }
#line 6773 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 464:
#line 2263 "cf-parse.y" /* yacc.c:1646  */
    {
     struct f_inst *tmp = f_new_inst();
     tmp->code = 'c';
     tmp->aux = T_ROLE;
     tmp->a2.i = (yyvsp[-1].i);
     (yyval.x) = f_new_inst();
     (yyval.x)->code = 'r';
     (yyval.x)->a1.p = tmp;
   }
#line 6787 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 465:
#line 2273 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     DBG( "Ook, we'll return the value\n" );
     (yyval.x)->code = 'r';
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6798 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 466:
#line 2279 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = (yyvsp[-3].x);
     (yyval.x)->code = P('e','S');
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6808 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 467:
#line 2284 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = (yyvsp[-3].x);
     if (!(yyval.x)->a1.i)
       cf_error( "This static attribute is read-only.");
     (yyval.x)->code = P('a','S');
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6820 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 468:
#line 2291 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     (yyval.x)->code = P('P','S');
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6830 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 469:
#line 2296 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = (yyvsp[-2].x);
     (yyval.x)->aux = EAF_TYPE_UNDEF | EAF_TEMP;
     (yyval.x)->code = P('e','S');
     (yyval.x)->a1.p = NULL;
   }
#line 6841 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 470:
#line 2302 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('p',','); (yyval.x)->a1.p = (yyvsp[-1].x); (yyval.x)->a2.i = (yyvsp[-2].i); }
#line 6847 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 471:
#line 2303 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[-1].x); }
#line 6853 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 472:
#line 2304 "cf-parse.y" /* yacc.c:1646  */
    {
      (yyval.x) = f_new_inst();
      (yyval.x)->code = P('S','W');
      (yyval.x)->a1.p = (yyvsp[-3].x);
      (yyval.x)->a2.p = build_tree( (yyvsp[-1].e) );
   }
#line 6864 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 473:
#line 2312 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_empty((yyvsp[-3].x)); }
#line 6870 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 474:
#line 2313 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('A','p'), 'x', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6876 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 475:
#line 2314 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('C','a'), 'a', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6882 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 476:
#line 2315 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('C','a'), 'd', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6888 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 477:
#line 2316 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('C','a'), 'f', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6894 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 478:
#line 2323 "cf-parse.y" /* yacc.c:1646  */
    {
  this_proto = proto_config_new(&proto_bfd, (yyvsp[-1].i));
  init_list(&BFD_CFG->patt_list);
  init_list(&BFD_CFG->neigh_list);

  if (bfd_cf)
    cf_error("Only one BFD instance allowed");
  bfd_cf = BFD_CFG;
}
#line 6908 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 486:
#line 2350 "cf-parse.y" /* yacc.c:1646  */
    {
  this_ipatt = cfg_allocz(sizeof(struct bfd_iface_config));
  init_list(&this_ipatt->ipn_list);

  BFD_IFACE->min_rx_int = BFD_DEFAULT_MIN_RX_INT;
  BFD_IFACE->min_tx_int = BFD_DEFAULT_MIN_TX_INT;
  BFD_IFACE->idle_tx_int = BFD_DEFAULT_IDLE_TX_INT;
  BFD_IFACE->multiplier = BFD_DEFAULT_MULTIPLIER;
}
#line 6922 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 487:
#line 2361 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->min_rx_int = BFD_IFACE->min_tx_int = (yyvsp[0].i32); }
#line 6928 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 488:
#line 2362 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->min_rx_int = (yyvsp[0].i32); }
#line 6934 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 489:
#line 2363 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->min_tx_int = (yyvsp[0].i32); }
#line 6940 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 490:
#line 2364 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->idle_tx_int = (yyvsp[0].i32); }
#line 6946 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 491:
#line 2365 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->multiplier = (yyvsp[0].i); }
#line 6952 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 492:
#line 2366 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->passive = (yyvsp[0].i); }
#line 6958 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 497:
#line 2380 "cf-parse.y" /* yacc.c:1646  */
    { add_tail(&BFD_CFG->patt_list, NODE this_ipatt); }
#line 6964 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 498:
#line 2383 "cf-parse.y" /* yacc.c:1646  */
    { BFD_CFG->multihop = BFD_IFACE; }
#line 6970 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 499:
#line 2387 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = NULL; }
#line 6976 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 500:
#line 2388 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = if_get_by_name((yyvsp[0].s)->name); }
#line 6982 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 501:
#line 2389 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = if_get_by_name((yyvsp[0].t)); }
#line 6988 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 502:
#line 2393 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.a) = IPA_NONE; }
#line 6994 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 503:
#line 2394 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.a) = (yyvsp[0].a); }
#line 7000 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 504:
#line 2398 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 7006 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 505:
#line 2399 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 7012 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 506:
#line 2403 "cf-parse.y" /* yacc.c:1646  */
    {
  this_bfd_neighbor = cfg_allocz(sizeof(struct bfd_neighbor));
  add_tail(&BFD_CFG->neigh_list, NODE this_bfd_neighbor);

  BFD_NEIGHBOR->addr = (yyvsp[-3].a);
  BFD_NEIGHBOR->local = (yyvsp[-1].a);
  BFD_NEIGHBOR->iface = (yyvsp[-2].iface);
  BFD_NEIGHBOR->multihop = (yyvsp[0].i);

  if ((yyvsp[0].i) && (yyvsp[-2].iface))
    cf_error("Neighbor cannot set both interface and multihop");

  if ((yyvsp[0].i) && ipa_zero((yyvsp[-1].a)))
    cf_error("Multihop neighbor requires specified local address");
}
#line 7032 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 507:
#line 2422 "cf-parse.y" /* yacc.c:1646  */
    { bfd_show_sessions(proto_get_named((yyvsp[-1].s), &proto_bfd)); }
#line 7038 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 508:
#line 2427 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_bgp, (yyvsp[-1].i));
     BGP_CFG->remote_port = BGP_PORT;
     BGP_CFG->multihop = -1;	/* undefined */
     BGP_CFG->hold_time = 240;
     BGP_CFG->initial_hold_time = 240;
     BGP_CFG->compare_path_lengths = 1;
     BGP_CFG->igp_metric = 1;
     BGP_CFG->connect_delay_time = 5;
     BGP_CFG->connect_retry_time = 120;
     BGP_CFG->error_amnesia_time = 300;
     BGP_CFG->error_delay_time_min = 60;
     BGP_CFG->error_delay_time_max = 300;
     BGP_CFG->enable_refresh = 1;
     BGP_CFG->enable_as4 = 1;
     BGP_CFG->capabilities = 2;
     BGP_CFG->advertise_ipv4 = 1;
     BGP_CFG->interpret_communities = 1;
     BGP_CFG->default_local_pref = 100;
     BGP_CFG->gr_mode = BGP_GR_AWARE;
     BGP_CFG->gr_time = 120;
     BGP_CFG->role = ROLE_UNDE;
     BGP_CFG->strict_mode = 0;
     BGP_CFG->role_map = NULL;
 }
#line 7068 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 510:
#line 2456 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->remote_port = (yyvsp[0].i); if (((yyvsp[0].i)<1) || ((yyvsp[0].i)>65535)) cf_error("Invalid port number");  }
#line 7074 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 511:
#line 2457 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->remote_as = (yyvsp[0].i); }
#line 7080 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 514:
#line 2463 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->local_as = (yyvsp[-1].i); }
#line 7086 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 515:
#line 2464 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->source_addr = (yyvsp[-3].a); BGP_CFG->local_as = (yyvsp[-1].i); }
#line 7092 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 517:
#line 2466 "cf-parse.y" /* yacc.c:1646  */
    {
     if (ipa_nonzero(BGP_CFG->remote_ip))
       cf_error("Only one neighbor per BGP instance is allowed");
     BGP_CFG->remote_ip = (yyvsp[-3].a);
     if ((yyvsp[-2].iface)) BGP_CFG->iface = (yyvsp[-2].iface);
   }
#line 7103 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 518:
#line 2472 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->iface = if_get_by_name((yyvsp[-1].t)); }
#line 7109 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 519:
#line 2473 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->rr_cluster_id = (yyvsp[-1].i32); }
#line 7115 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 520:
#line 2474 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->rr_client = 1; }
#line 7121 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 521:
#line 2475 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->rs_client = 1; }
#line 7127 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 522:
#line 2476 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->hold_time = (yyvsp[-1].i); }
#line 7133 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 523:
#line 2477 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->initial_hold_time = (yyvsp[-1].i); }
#line 7139 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 524:
#line 2478 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->multihop = 0; }
#line 7145 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 525:
#line 2479 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->multihop = 64; }
#line 7151 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 526:
#line 2480 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->multihop = (yyvsp[-1].i); if (((yyvsp[-1].i)<1) || ((yyvsp[-1].i)>255)) cf_error("Multihop must be in range 1-255"); }
#line 7157 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 527:
#line 2481 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->next_hop_self = 1; BGP_CFG->next_hop_keep = 0; }
#line 7163 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 528:
#line 2482 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->next_hop_keep = 1; BGP_CFG->next_hop_self = 0; }
#line 7169 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 529:
#line 2483 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->missing_lladdr = MLL_SELF; }
#line 7175 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 530:
#line 2484 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->missing_lladdr = MLL_DROP; }
#line 7181 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 531:
#line 2485 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->missing_lladdr = MLL_IGNORE; }
#line 7187 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 532:
#line 2486 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gw_mode = GW_DIRECT; }
#line 7193 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 533:
#line 2487 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gw_mode = GW_RECURSIVE; }
#line 7199 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 534:
#line 2488 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->compare_path_lengths = (yyvsp[-1].i); }
#line 7205 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 535:
#line 2489 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->med_metric = (yyvsp[-1].i); }
#line 7211 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 536:
#line 2490 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->igp_metric = (yyvsp[-1].i); }
#line 7217 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 537:
#line 2491 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->prefer_older = (yyvsp[-1].i); }
#line 7223 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 538:
#line 2492 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->deterministic_med = (yyvsp[-1].i); }
#line 7229 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 539:
#line 2493 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->default_med = (yyvsp[-1].i); }
#line 7235 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 540:
#line 2494 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->default_local_pref = (yyvsp[-1].i); }
#line 7241 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 541:
#line 2495 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->source_addr = (yyvsp[-1].a); }
#line 7247 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 542:
#line 2496 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->connect_delay_time = (yyvsp[-1].i); log(L_WARN "%s: Start delay time option is deprecated, use connect delay time", this_proto->name); }
#line 7253 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 543:
#line 2497 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->connect_delay_time = (yyvsp[-1].i); }
#line 7259 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 544:
#line 2498 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->connect_retry_time = (yyvsp[-1].i); }
#line 7265 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 545:
#line 2499 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->keepalive_time = (yyvsp[-1].i); }
#line 7271 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 546:
#line 2500 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->error_amnesia_time = (yyvsp[-1].i); }
#line 7277 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 547:
#line 2501 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->error_delay_time_min = (yyvsp[-3].i); BGP_CFG->error_delay_time_max = (yyvsp[-1].i); }
#line 7283 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 548:
#line 2502 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->disable_after_error = (yyvsp[-1].i); }
#line 7289 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 549:
#line 2503 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->enable_refresh = (yyvsp[-1].i); }
#line 7295 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 550:
#line 2504 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->enable_as4 = (yyvsp[-1].i); }
#line 7301 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 551:
#line 2505 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->enable_extended_messages = (yyvsp[-1].i); }
#line 7307 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 552:
#line 2506 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->capabilities = (yyvsp[-1].i); }
#line 7313 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 553:
#line 2507 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->advertise_ipv4 = (yyvsp[-1].i); }
#line 7319 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 554:
#line 2508 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->password = (yyvsp[-1].t); }
#line 7325 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 555:
#line 2509 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto->in_limit = cfg_allocz(sizeof(struct proto_limit));
     this_proto->in_limit->limit = (yyvsp[-1].i);
     this_proto->in_limit->action = PLA_RESTART;
     log(L_WARN "%s: Route limit option is deprecated, use import limit", this_proto->name);
   }
#line 7336 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 556:
#line 2515 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->passive = (yyvsp[-1].i); }
#line 7342 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 557:
#line 2516 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->interpret_communities = (yyvsp[-1].i); }
#line 7348 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 558:
#line 2517 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->secondary = (yyvsp[-1].i); }
#line 7354 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 559:
#line 2518 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->add_path = ADD_PATH_RX; }
#line 7360 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 560:
#line 2519 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->add_path = ADD_PATH_TX; }
#line 7366 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 561:
#line 2520 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->add_path = (yyvsp[-1].i) ? ADD_PATH_FULL : 0; }
#line 7372 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 562:
#line 2521 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->allow_local_as = -1; }
#line 7378 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 563:
#line 2522 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->allow_local_as = (yyvsp[-1].i); }
#line 7384 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 564:
#line 2523 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gr_mode = (yyvsp[-1].i); }
#line 7390 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 565:
#line 2524 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gr_mode = BGP_GR_AWARE; }
#line 7396 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 566:
#line 2525 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gr_time = (yyvsp[-1].i); }
#line 7402 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 567:
#line 2526 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->igp_table = (yyvsp[-1].r); }
#line 7408 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 568:
#line 2527 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->ttl_security = (yyvsp[-1].i); }
#line 7414 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 569:
#line 2528 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->check_link = (yyvsp[-1].i); }
#line 7420 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 570:
#line 2529 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->bfd = (yyvsp[-1].i); cf_check_bfd((yyvsp[-1].i)); }
#line 7426 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 571:
#line 2530 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->role = (yyvsp[-1].i); }
#line 7432 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 572:
#line 2531 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->role = ROLE_COMP; BGP_CFG->role_map = (yyvsp[-1].rm); }
#line 7438 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 573:
#line 2532 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->strict_mode = 1; }
#line 7444 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 574:
#line 2543 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_ospf, (yyvsp[-1].i));
     init_list(&OSPF_CFG->area_list);
     init_list(&OSPF_CFG->vlink_list);
     OSPF_CFG->tick = OSPF_DEFAULT_TICK;
     OSPF_CFG->ospf2 = OSPF_IS_V2;
  }
#line 7456 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 578:
#line 2559 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->rfc1583 = (yyvsp[0].i); }
#line 7462 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 579:
#line 2560 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->stub_router = (yyvsp[0].i); }
#line 7468 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 580:
#line 2561 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->ecmp = (yyvsp[0].i) ? OSPF_DEFAULT_ECMP_LIMIT : 0; }
#line 7474 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 581:
#line 2562 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->ecmp = (yyvsp[-2].i) ? (yyvsp[0].i) : 0; if ((yyvsp[0].i) < 0) cf_error("ECMP limit cannot be negative"); }
#line 7480 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 582:
#line 2563 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->merge_external = (yyvsp[0].i); }
#line 7486 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 583:
#line 2564 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->tick = (yyvsp[0].i); if((yyvsp[0].i)<=0) cf_error("Tick must be greater than zero"); }
#line 7492 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 584:
#line 2565 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->instance_id = (yyvsp[0].i); if (((yyvsp[0].i)<0) || ((yyvsp[0].i)>255)) cf_error("Instance ID must be in range 0-255"); }
#line 7498 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 586:
#line 2569 "cf-parse.y" /* yacc.c:1646  */
    {
  this_area = cfg_allocz(sizeof(struct ospf_area_config));
  add_tail(&OSPF_CFG->area_list, NODE this_area);
  this_area->areaid = (yyvsp[0].i32);
  this_area->default_cost = OSPF_DEFAULT_STUB_COST;
  this_area->type = OPT_E;
  this_area->transint = OSPF_DEFAULT_TRANSINT;

  init_list(&this_area->patt_list);
  init_list(&this_area->net_list);
  init_list(&this_area->enet_list);
  init_list(&this_area->stubnet_list);
 }
#line 7516 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 587:
#line 2584 "cf-parse.y" /* yacc.c:1646  */
    { ospf_area_finish(); }
#line 7522 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 590:
#line 2593 "cf-parse.y" /* yacc.c:1646  */
    { this_area->type = (yyvsp[0].i) ? 0 : OPT_E; /* We should remove the option */ }
#line 7528 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 591:
#line 2594 "cf-parse.y" /* yacc.c:1646  */
    { this_area->type = OPT_N; }
#line 7534 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 592:
#line 2595 "cf-parse.y" /* yacc.c:1646  */
    { this_area->summary = (yyvsp[0].i); }
#line 7540 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 593:
#line 2596 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_nssa = (yyvsp[0].i); }
#line 7546 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 594:
#line 2597 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_cost = (yyvsp[0].i); ospf_check_defcost((yyvsp[0].i)); }
#line 7552 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 595:
#line 2598 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_cost = (yyvsp[0].i) | LSA_EXT3_EBIT; ospf_check_defcost((yyvsp[0].i)); }
#line 7558 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 596:
#line 2599 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_cost = (yyvsp[0].i); ospf_check_defcost((yyvsp[0].i)); }
#line 7564 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 597:
#line 2600 "cf-parse.y" /* yacc.c:1646  */
    { this_area->translator = (yyvsp[0].i); }
#line 7570 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 598:
#line 2601 "cf-parse.y" /* yacc.c:1646  */
    { this_area->transint = (yyvsp[0].i); }
#line 7576 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 599:
#line 2602 "cf-parse.y" /* yacc.c:1646  */
    { this_nets = &this_area->net_list; }
#line 7582 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 601:
#line 2603 "cf-parse.y" /* yacc.c:1646  */
    { this_nets = &this_area->enet_list; }
#line 7588 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 608:
#line 2615 "cf-parse.y" /* yacc.c:1646  */
    {
     this_stubnet = cfg_allocz(sizeof(struct ospf_stubnet_config));
     add_tail(&this_area->stubnet_list, NODE this_stubnet);
     this_stubnet->px = (yyvsp[0].px);
     this_stubnet->cost = COST_D;
   }
#line 7599 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 611:
#line 2629 "cf-parse.y" /* yacc.c:1646  */
    { this_stubnet->hidden = (yyvsp[0].i); }
#line 7605 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 612:
#line 2630 "cf-parse.y" /* yacc.c:1646  */
    { this_stubnet->summary = (yyvsp[0].i); }
#line 7611 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 613:
#line 2631 "cf-parse.y" /* yacc.c:1646  */
    { this_stubnet->cost = (yyvsp[0].i); }
#line 7617 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 614:
#line 2635 "cf-parse.y" /* yacc.c:1646  */
    { ospf_iface_finish(); }
#line 7623 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 615:
#line 2636 "cf-parse.y" /* yacc.c:1646  */
    { ospf_iface_finish(); }
#line 7629 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 619:
#line 2645 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->helloint = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Hello interval must be in range 1-65535"); }
#line 7635 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 620:
#line 2646 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rxmtint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Retransmit int must be greater than one"); }
#line 7641 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 621:
#line 2647 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->inftransdelay = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Transmit delay must be in range 1-65535"); }
#line 7647 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 622:
#line 2648 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->waitint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Wait interval must be greater than one"); }
#line 7653 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 623:
#line 2649 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead interval must be greater than one"); }
#line 7659 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 624:
#line 2650 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadc = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead count must be greater than one"); }
#line 7665 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 625:
#line 2651 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_NONE;  }
#line 7671 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 626:
#line 2652 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_SIMPLE; ospf_check_auth(); }
#line 7677 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 627:
#line 2653 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_CRYPT; ospf_check_auth(); }
#line 7683 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 628:
#line 2654 "cf-parse.y" /* yacc.c:1646  */
    { ospf_check_auth(); }
#line 7689 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 629:
#line 2658 "cf-parse.y" /* yacc.c:1646  */
    {
  if (this_area->areaid == 0) cf_error("Virtual link cannot be in backbone");
  this_ipatt = cfg_allocz(sizeof(struct ospf_iface_patt));
  add_tail(&OSPF_CFG->vlink_list, NODE this_ipatt);
  init_list(&this_ipatt->ipn_list);
  OSPF_PATT->voa = this_area->areaid;
  OSPF_PATT->vid = (yyvsp[0].i32);
  OSPF_PATT->helloint = HELLOINT_D;
  OSPF_PATT->rxmtint = RXMTINT_D;
  OSPF_PATT->inftransdelay = INFTRANSDELAY_D;
  OSPF_PATT->deadc = DEADC_D;
  OSPF_PATT->type = OSPF_IT_VLINK;
  OSPF_PATT->instance_id = OSPF_CFG->instance_id;
  init_list(&OSPF_PATT->nbma_list);
  reset_passwords();
 }
#line 7710 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 630:
#line 2677 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->cost = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Cost must be in range 1-65535"); }
#line 7716 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 631:
#line 2678 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->helloint = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Hello interval must be in range 1-65535"); }
#line 7722 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 632:
#line 2679 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->pollint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=0) cf_error("Poll int must be greater than zero"); }
#line 7728 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 633:
#line 2680 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rxmtint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Retransmit int must be greater than one"); }
#line 7734 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 634:
#line 2681 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->waitint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Wait interval must be greater than one"); }
#line 7740 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 635:
#line 2682 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead interval must be greater than one"); }
#line 7746 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 636:
#line 2683 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadc = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead count must be greater than one"); }
#line 7752 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 637:
#line 2684 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_BCAST ; }
#line 7758 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 638:
#line 2685 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_BCAST ; }
#line 7764 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 639:
#line 2686 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_NBMA ; }
#line 7770 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 640:
#line 2687 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_NBMA ; }
#line 7776 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 641:
#line 2688 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTP ; }
#line 7782 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 642:
#line 2689 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTP ; }
#line 7788 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 643:
#line 2690 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTMP ; }
#line 7794 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 644:
#line 2691 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTMP ; }
#line 7800 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 645:
#line 2692 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->real_bcast = (yyvsp[0].i); if (!ospf_cfg_is_v2()) cf_error("Real broadcast option requires OSPFv2"); }
#line 7806 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 646:
#line 2693 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ptp_netmask = (yyvsp[0].i); if (!ospf_cfg_is_v2()) cf_error("PtP netmask option requires OSPFv2"); }
#line 7812 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 647:
#line 2694 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->inftransdelay = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Transmit delay must be in range 1-65535"); }
#line 7818 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 648:
#line 2695 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->priority = (yyvsp[0].i) ; if (((yyvsp[0].i)<0) || ((yyvsp[0].i)>255)) cf_error("Priority must be in range 0-255"); }
#line 7824 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 649:
#line 2696 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->strictnbma = (yyvsp[0].i) ; }
#line 7830 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 650:
#line 2697 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->stub = (yyvsp[0].i) ; }
#line 7836 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 651:
#line 2698 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->check_link = (yyvsp[0].i); }
#line 7842 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 652:
#line 2699 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ecmp_weight = (yyvsp[0].i) - 1; if (((yyvsp[0].i)<1) || ((yyvsp[0].i)>256)) cf_error("ECMP weight must be in range 1-256"); }
#line 7848 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 653:
#line 2700 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->link_lsa_suppression = (yyvsp[0].i); if (!ospf_cfg_is_v3()) cf_error("Link LSA suppression option requires OSPFv3"); }
#line 7854 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 655:
#line 2702 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_NONE; }
#line 7860 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 656:
#line 2703 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_SIMPLE; ospf_check_auth(); }
#line 7866 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 657:
#line 2704 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_CRYPT; ospf_check_auth(); }
#line 7872 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 658:
#line 2705 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rx_buffer = 0; }
#line 7878 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 659:
#line 2706 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rx_buffer = OSPF_MAX_PKT_SIZE; }
#line 7884 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 660:
#line 2707 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rx_buffer = (yyvsp[0].i); if (((yyvsp[0].i) < OSPF_MIN_PKT_SIZE) || ((yyvsp[0].i) > OSPF_MAX_PKT_SIZE)) cf_error("Buffer size must be in range 256-65535"); }
#line 7890 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 661:
#line 2708 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->tx_tos = (yyvsp[0].i); }
#line 7896 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 662:
#line 2709 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->tx_priority = (yyvsp[0].i); }
#line 7902 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 663:
#line 2710 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->tx_length = (yyvsp[0].i); if (((yyvsp[0].i) < OSPF_MIN_PKT_SIZE) || ((yyvsp[0].i) > OSPF_MAX_PKT_SIZE)) cf_error("TX length must be in range 256-65535"); }
#line 7908 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 664:
#line 2711 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ttl_security = (yyvsp[0].i); }
#line 7914 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 665:
#line 2712 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ttl_security = 2; }
#line 7920 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 666:
#line 2713 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->bfd = (yyvsp[0].i); cf_check_bfd((yyvsp[0].i)); }
#line 7926 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 667:
#line 2714 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->bsd_secondary = (yyvsp[0].i); }
#line 7932 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 668:
#line 2715 "cf-parse.y" /* yacc.c:1646  */
    { ospf_check_auth(); }
#line 7938 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 672:
#line 2726 "cf-parse.y" /* yacc.c:1646  */
    {
   this_pref = cfg_allocz(sizeof(struct area_net_config));
   add_tail(this_nets, NODE this_pref);
   this_pref->px.addr = (yyvsp[0].px).addr;
   this_pref->px.len = (yyvsp[0].px).len;
 }
#line 7949 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 674:
#line 2736 "cf-parse.y" /* yacc.c:1646  */
    { this_pref->hidden = 1; }
#line 7955 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 675:
#line 2737 "cf-parse.y" /* yacc.c:1646  */
    { this_pref->tag = (yyvsp[0].i); }
#line 7961 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 678:
#line 2746 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 7967 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 679:
#line 2747 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 7973 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 680:
#line 2751 "cf-parse.y" /* yacc.c:1646  */
    {
   this_nbma = cfg_allocz(sizeof(struct nbma_node));
   add_tail(&OSPF_PATT->nbma_list, NODE this_nbma);
   this_nbma->ip=(yyvsp[-2].a);
   this_nbma->eligible=(yyvsp[-1].i);
 }
#line 7984 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 681:
#line 2760 "cf-parse.y" /* yacc.c:1646  */
    {
  this_ipatt = cfg_allocz(sizeof(struct ospf_iface_patt));
  add_tail(&this_area->patt_list, NODE this_ipatt);
  init_list(&this_ipatt->ipn_list);
  OSPF_PATT->cost = COST_D;
  OSPF_PATT->helloint = HELLOINT_D;
  OSPF_PATT->pollint = POLLINT_D;
  OSPF_PATT->rxmtint = RXMTINT_D;
  OSPF_PATT->inftransdelay = INFTRANSDELAY_D;
  OSPF_PATT->priority = PRIORITY_D;
  OSPF_PATT->deadc = DEADC_D;
  OSPF_PATT->type = OSPF_IT_UNDEF;
  OSPF_PATT->instance_id = OSPF_CFG->instance_id;
  init_list(&OSPF_PATT->nbma_list);
  OSPF_PATT->ptp_netmask = 2; /* not specified */
  OSPF_PATT->tx_tos = IP_PREC_INTERNET_CONTROL;
  OSPF_PATT->tx_priority = sk_priority_control;
  reset_passwords();
 }
#line 8008 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 683:
#line 2783 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->instance_id = (yyvsp[0].i); if (((yyvsp[0].i)<0) || ((yyvsp[0].i)>255)) cf_error("Instance ID must be in range 0-255"); }
#line 8014 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 684:
#line 2787 "cf-parse.y" /* yacc.c:1646  */
    { if (ospf_cfg_is_v3()) iface_patt_check(); }
#line 8020 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 690:
#line 2801 "cf-parse.y" /* yacc.c:1646  */
    { ospf_iface_finish(); }
#line 8026 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 692:
#line 2806 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = NULL; }
#line 8032 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 693:
#line 2812 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh(proto_get_named((yyvsp[-1].s), &proto_ospf)); }
#line 8038 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 694:
#line 2815 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_neigh(proto_get_named((yyvsp[-2].s), &proto_ospf), (yyvsp[-1].t)); }
#line 8044 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 695:
#line 2818 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_iface(proto_get_named((yyvsp[-2].s), &proto_ospf), (yyvsp[-1].t)); }
#line 8050 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 696:
#line 2823 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 0, 1); }
#line 8056 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 697:
#line 2826 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 0, 0); }
#line 8062 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 698:
#line 2831 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 1, 1); }
#line 8068 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 699:
#line 2834 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 1, 0); }
#line 8074 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 700:
#line 2838 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_lsadb((yyvsp[-1].ld)); }
#line 8080 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 701:
#line 2841 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ld) = cfg_allocz(sizeof(struct lsadb_show_data));
   }
#line 8088 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 702:
#line 2844 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->scope = LSA_SCOPE_AS; }
#line 8094 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 703:
#line 2845 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->scope = LSA_SCOPE_AREA; (yyval.ld)->area = (yyvsp[0].i32); }
#line 8100 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 704:
#line 2846 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->scope = 1; /* hack, 0 is no filter */ }
#line 8106 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 705:
#line 2847 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->type = (yyvsp[0].i); }
#line 8112 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 706:
#line 2848 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->lsid = (yyvsp[0].i32); }
#line 8118 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 707:
#line 2849 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->router = SH_ROUTER_SELF; }
#line 8124 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 708:
#line 2850 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->router = (yyvsp[0].i32); }
#line 8130 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 709:
#line 2851 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->name = (yyvsp[0].s); }
#line 8136 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 710:
#line 2857 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_pipe, (yyvsp[-1].i));
     PIPE_CFG->mode = PIPE_TRANSPARENT;
  }
#line 8145 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 713:
#line 2866 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[-1].s)->class != SYM_TABLE)
       cf_error("Routing table name expected");
     PIPE_CFG->peer = (yyvsp[-1].s)->def;
   }
#line 8155 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 714:
#line 2871 "cf-parse.y" /* yacc.c:1646  */
    { PIPE_CFG->mode = PIPE_OPAQUE; }
#line 8161 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 715:
#line 2872 "cf-parse.y" /* yacc.c:1646  */
    { PIPE_CFG->mode = PIPE_TRANSPARENT; }
#line 8167 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 716:
#line 2878 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_rip, (yyvsp[-1].i));
     rip_init_config(RIP_CFG);
   }
#line 8176 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 719:
#line 2887 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->infinity = (yyvsp[-1].i); }
#line 8182 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 720:
#line 2888 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->port = (yyvsp[-1].i); }
#line 8188 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 721:
#line 2889 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->period = (yyvsp[-1].i); }
#line 8194 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 722:
#line 2890 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->garbage_time = (yyvsp[-1].i); }
#line 8200 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 723:
#line 2891 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->timeout_time = (yyvsp[-1].i); }
#line 8206 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 724:
#line 2892 "cf-parse.y" /* yacc.c:1646  */
    {RIP_CFG->authtype = (yyvsp[-1].i); }
#line 8212 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 726:
#line 2894 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->honor = HO_ALWAYS; }
#line 8218 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 727:
#line 2895 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->honor = HO_NEIGHBOR; }
#line 8224 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 728:
#line 2896 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->honor = HO_NEVER; }
#line 8230 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 730:
#line 2901 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=AT_PLAINTEXT; }
#line 8236 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 731:
#line 2902 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=AT_MD5; }
#line 8242 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 732:
#line 2903 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=AT_NONE; }
#line 8248 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 733:
#line 2908 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_BROADCAST; }
#line 8254 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 734:
#line 2909 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=0; }
#line 8260 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 735:
#line 2910 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_QUIET; }
#line 8266 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 736:
#line 2911 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_NOLISTEN; }
#line 8272 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 737:
#line 2912 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_VERSION1 | IM_BROADCAST; }
#line 8278 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 739:
#line 2916 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->metric = (yyvsp[0].i); }
#line 8284 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 740:
#line 2917 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->mode |= (yyvsp[0].i); }
#line 8290 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 741:
#line 2918 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->tx_tos = (yyvsp[0].i); }
#line 8296 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 742:
#line 2919 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->tx_priority = (yyvsp[0].i); }
#line 8302 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 743:
#line 2920 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->ttl_security = (yyvsp[0].i); }
#line 8308 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 744:
#line 2921 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->ttl_security = 2; }
#line 8314 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 749:
#line 2935 "cf-parse.y" /* yacc.c:1646  */
    {
     this_ipatt = cfg_allocz(sizeof(struct rip_patt));
     add_tail(&RIP_CFG->iface_list, NODE this_ipatt);
     init_list(&this_ipatt->ipn_list);
     RIP_IPATT->metric = 1;
     RIP_IPATT->tx_tos = IP_PREC_INTERNET_CONTROL;
     RIP_IPATT->tx_priority = sk_priority_control;
     RIP_IPATT->ttl_security = RIP_DEFAULT_TTL_SECURITY;
   }
#line 8328 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 751:
#line 2954 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_static, (yyvsp[-1].i));
     static_init_config((struct static_config *) this_proto);
  }
#line 8337 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 754:
#line 2963 "cf-parse.y" /* yacc.c:1646  */
    { STATIC_CFG->check_link = (yyvsp[-1].i); }
#line 8343 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 755:
#line 2964 "cf-parse.y" /* yacc.c:1646  */
    { STATIC_CFG->igp_table = (yyvsp[-1].r); }
#line 8349 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 756:
#line 2965 "cf-parse.y" /* yacc.c:1646  */
    { static_route_finish(); }
#line 8355 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 757:
#line 2968 "cf-parse.y" /* yacc.c:1646  */
    {
     this_srt = cfg_allocz(sizeof(struct static_route));
     add_tail(&STATIC_CFG->other_routes, &this_srt->n);
     this_srt->net = (yyvsp[0].px).addr;
     this_srt->masklen = (yyvsp[0].px).len;
     this_srt_last_cmd = &(this_srt->cmds);
  }
#line 8367 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 758:
#line 2978 "cf-parse.y" /* yacc.c:1646  */
    {
     last_srt_nh = this_srt_nh;
     this_srt_nh = cfg_allocz(sizeof(struct static_route));
     this_srt_nh->dest = RTD_NONE;
     this_srt_nh->via = (yyvsp[-1].a);
     this_srt_nh->via_if = (yyvsp[0].iface);
     this_srt_nh->if_name = (void *) this_srt; /* really */
     this_srt_nh->use_bfd = -1; /* undefined */
   }
#line 8381 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 759:
#line 2987 "cf-parse.y" /* yacc.c:1646  */
    {
     this_srt_nh->masklen = (yyvsp[0].i) - 1; /* really */
     if (((yyvsp[0].i)<1) || ((yyvsp[0].i)>256)) cf_error("Weight must be in range 1-256"); 
   }
#line 8390 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 760:
#line 2991 "cf-parse.y" /* yacc.c:1646  */
    {
     this_srt_nh->use_bfd = (yyvsp[0].i); cf_check_bfd((yyvsp[0].i));
   }
#line 8398 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 761:
#line 2997 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->mp_next = this_srt_nh; }
#line 8404 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 762:
#line 2998 "cf-parse.y" /* yacc.c:1646  */
    { last_srt_nh->mp_next = this_srt_nh; }
#line 8410 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 763:
#line 3002 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTD_ROUTER;
      this_srt->via = (yyvsp[-1].a);
      this_srt->via_if = (yyvsp[0].iface);
   }
#line 8420 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 764:
#line 3007 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTD_DEVICE;
      this_srt->if_name = (yyvsp[0].t);
      rem_node(&this_srt->n);
      add_tail(&STATIC_CFG->iface_routes, &this_srt->n);
   }
#line 8431 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 765:
#line 3013 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTD_MULTIPATH;
   }
#line 8439 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 766:
#line 3016 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTDX_RECURSIVE;
      this_srt->via = (yyvsp[0].a);
   }
#line 8448 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 767:
#line 3021 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_BLACKHOLE; }
#line 8454 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 768:
#line 3022 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_UNREACHABLE; }
#line 8460 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 769:
#line 3023 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_BLACKHOLE; }
#line 8466 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 770:
#line 3024 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_UNREACHABLE; }
#line 8472 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 771:
#line 3025 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_PROHIBIT; }
#line 8478 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 772:
#line 3029 "cf-parse.y" /* yacc.c:1646  */
    { *this_srt_last_cmd = (yyvsp[0].x); this_srt_last_cmd = &((yyvsp[0].x)->next); }
#line 8484 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 773:
#line 3030 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->use_bfd = (yyvsp[-1].i); cf_check_bfd((yyvsp[-1].i)); }
#line 8490 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 778:
#line 3045 "cf-parse.y" /* yacc.c:1646  */
    { static_show(proto_get_named((yyvsp[-1].s), &proto_static)); }
#line 8496 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 847:
#line 3053 "cf-parse.y" /* yacc.c:1646  */
    { bgp_check_config(BGP_CFG); }
#line 8502 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 848:
#line 3053 "cf-parse.y" /* yacc.c:1646  */
    { ospf_proto_finish(); }
#line 8508 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 850:
#line 3053 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->passwords = get_passwords(); }
#line 8514 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 859:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_KRT_SOURCE); }
#line 8520 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 860:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_KRT_METRIC); }
#line 8526 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 861:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_IP_ADDRESS, T_IP, EA_KRT_PREFSRC); }
#line 8532 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 862:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_REALM); }
#line 8538 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 863:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_MTU); }
#line 8544 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 864:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_WINDOW); }
#line 8550 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 865:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_RTT); }
#line 8556 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 866:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_RTTVAR); }
#line 8562 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 867:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_SSTRESH); }
#line 8568 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 868:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_CWND); }
#line 8574 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 869:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_ADVMSS); }
#line 8580 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 870:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_REORDERING); }
#line 8586 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 871:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_HOPLIMIT); }
#line 8592 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 872:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_INITCWND); }
#line 8598 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 873:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_RTO_MIN); }
#line 8604 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 874:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_INITRWND); }
#line 8610 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 875:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_QUICKACK); }
#line 8616 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 876:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_MTU); }
#line 8622 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 877:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_WINDOW); }
#line 8628 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 878:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_RTT); }
#line 8634 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 879:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_RTTVAR); }
#line 8640 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 880:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_SSTHRESH); }
#line 8646 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 881:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_CWND); }
#line 8652 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 882:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_ADVMSS); }
#line 8658 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 883:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_REORDERING); }
#line 8664 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 884:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_HOPLIMIT); }
#line 8670 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 885:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_LOCK_RTO_MIN); }
#line 8676 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 886:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_FEATURE_ECN); }
#line 8682 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 887:
#line 3056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_BITFIELD, T_BOOL, EA_KRT_FEATURE_ALLFRAG); }
#line 8688 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 888:
#line 3057 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_GEN_IGP_METRIC); }
#line 8694 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 889:
#line 3057 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 8700 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 890:
#line 3058 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_ENUM_BGP_ORIGIN, EA_CODE(EAP_BGP, BA_ORIGIN)); }
#line 8706 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 891:
#line 3059 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_AS_PATH, T_PATH, EA_CODE(EAP_BGP, BA_AS_PATH)); }
#line 8712 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 892:
#line 3060 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_IP_ADDRESS, T_IP, EA_CODE(EAP_BGP, BA_NEXT_HOP)); }
#line 8718 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 893:
#line 3061 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_MULTI_EXIT_DISC)); }
#line 8724 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 894:
#line 3062 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_LOCAL_PREF)); }
#line 8730 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 895:
#line 3063 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_OPAQUE, T_ENUM_EMPTY, EA_CODE(EAP_BGP, BA_ATOMIC_AGGR)); }
#line 8736 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 896:
#line 3064 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_AGGREGATOR)); }
#line 8742 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 897:
#line 3065 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT_SET, T_CLIST, EA_CODE(EAP_BGP, BA_COMMUNITY)); }
#line 8748 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 898:
#line 3066 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_ROUTER_ID, T_QUAD, EA_CODE(EAP_BGP, BA_ORIGINATOR_ID)); }
#line 8754 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 899:
#line 3067 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT_SET, T_CLIST, EA_CODE(EAP_BGP, BA_CLUSTER_LIST)); }
#line 8760 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 900:
#line 3068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_EC_SET, T_ECLIST, EA_CODE(EAP_BGP, BA_EXT_COMMUNITY)); }
#line 8766 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 901:
#line 3068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_METRIC1); }
#line 8772 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 902:
#line 3068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_METRIC2); }
#line 8778 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 903:
#line 3068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_TAG); }
#line 8784 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 904:
#line 3068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_ROUTER_ID | EAF_TEMP, T_QUAD, EA_OSPF_ROUTER_ID); }
#line 8790 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 905:
#line 3068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_RIP_METRIC); }
#line 8796 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 906:
#line 3068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_RIP_TAG); }
#line 8802 "cf-parse.tab.c" /* yacc.c:1646  */
    break;


#line 8806 "cf-parse.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 3070 "cf-parse.y" /* yacc.c:1906  */

/* C Code from ../../conf/confbase.Y */

/* C Code from ../../sysdep/unix/config.Y */

/* C Code from ../../sysdep/unix/krt.Y */

/* C Code from ../../sysdep/linux/netlink.Y */

/* C Code from ../../nest/config.Y */

/* C Code from ../../proto/bfd/config.Y */

/* C Code from ../../proto/bgp/config.Y */

/* C Code from ../../proto/ospf/config.Y */

/* C Code from ../../proto/pipe/config.Y */

/* C Code from ../../proto/rip/config.Y */

/* C Code from ../../proto/static/config.Y */

