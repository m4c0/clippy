#pragma leco tool
import dotz;
import getch;
import jute;
import print;
import rng;

typedef int numba;

static numba g_wire_spool = 1000;
static numba g_clips_per_box = 1;
static numba g_cost_per_box = 25;
static numba g_cost_per_spool = 1500;
static numba g_cost_per_autoclip = 0;
static numba g_demand = 32;

static numba g_paperclips = 0;
static numba g_wire = g_wire_spool;
static numba g_funds = 0;
static numba g_autoclips = 0;

static numba g_autoclip_min_funds = 500;

static constexpr const auto g_term_len = 5;
static jute::view g_term[g_term_len] {};

static void log(jute::view msg) {
  for (auto i = 1; i < g_term_len; i++) {
    g_term[i - 1] = g_term[i];
  }
  g_term[g_term_len - 1] = msg;
}

static bool autoclips() { return g_cost_per_autoclip != 0; }

static void sell() {
  if (g_paperclips < g_clips_per_box) return;
  if (rng::rand(100) < g_demand) return;
  g_paperclips -= g_clips_per_box;
  g_funds += g_cost_per_box;

  if (!autoclips() && g_funds > g_autoclip_min_funds) {
    g_cost_per_autoclip = g_autoclip_min_funds;
    log("Autoclippers enabled");
  }
}

static void autoclip() {
  if (!autoclips()) return;
  auto n = dotz::min(g_wire, g_autoclips);
  g_wire -= n;
  g_paperclips += n;
}

static void make_paperclip() {
  if (g_wire == 0) return log("Not enought wire");
  g_wire--;
  g_paperclips++;
}

static void buy_spool() {
  if (g_funds < g_cost_per_spool) return log("Not enough dindins");
  g_wire += g_wire_spool;
  g_funds -= g_cost_per_spool;
}

static void buy_autoclip() {
  if (!autoclips()) return;
  if (g_funds < g_cost_per_autoclip) return log("Not enough dindins");
  g_autoclips++;
  g_funds -= g_cost_per_autoclip;
}

static void tick() {
  sell();
  autoclip();
}

static void input() {
  switch (getch()) {
    case 'a': buy_autoclip(); break;
    case 'p': make_paperclip(); break;
    case 'w': buy_spool(); break;
    default:  break;
  }
}

static void draw() {
  putln("\e[1J");
  for (auto msg: g_term) putln(msg);
  putln();
  putln("Paperclips:   ", g_paperclips);
  putln("Wire:         ", g_wire, " units");
  putln("Funds:        ", g_funds, " dindins");
  putln("Autoclippers: ", g_autoclips);
  putln();
  putln("Cost of a paperclip box: ", g_cost_per_box);
  putln("Cost of a wire spool:    ", g_cost_per_spool);
  putln();
  putln("Press P to create a paperclip");
  putln("Press W to buy a wire spool");
  if (autoclips()) putln("Press A to buy an autoclipper");
}

static void cycle() {
  draw();
  tick();
  input();
}

int main() {
  while (true) cycle(); 
}
