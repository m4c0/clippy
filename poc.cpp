#pragma leco tool
import autoclipper;
import dotz;
import getch;
import log;
import numba;
import print;
import rng;
import save;

struct quit {};

static numba g_wire_spool = 1000;
static numba g_cost_per_box = 25;
static numba g_cost_per_spool = 1500;
static numba g_demand = 32;

static numba g_paperclips = 0;
static numba g_wire = g_wire_spool;
static numba g_funds = 0;

static void sell() {
  if (g_paperclips == 0) return;
  if (rng::rand(100) < g_demand) return;
  g_paperclips -= 1;
  g_funds += g_cost_per_box;

  autoclipper::check(g_funds);
}

static void autoclip() {
  auto n = autoclipper::run(g_wire);
  g_wire -= n;
  g_paperclips += n;
}

static void make_paperclip() {
  if (g_wire == 0) return;
  g_wire--;
  g_paperclips++;
}

static void buy_spool() {
  if (g_funds < g_cost_per_spool) return;
  g_wire += g_wire_spool;
  g_funds -= g_cost_per_spool;
}

static void buy_autoclip() {
  g_funds -= autoclipper::buy(g_funds);
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
    case 'q': throw quit {};
    default:  break;
  }
}

static void draw() {
  putln("\e[1J");
  log_print();
  putln();
  putln("Paperclips:   ", g_paperclips);
  putln("Wire:         ", g_wire, " units");
  putln("Funds:        ", g_funds, " dindins");
  if (autoclipper::enabled()) putln("Autoclippers: ", autoclipper::count());
  putln();
  putln("Costs of:");
  putln("* Paperclip box: ", g_cost_per_box);
  putln("* Wire spool:    ", g_cost_per_spool);
  if (autoclipper::enabled()) putln("* Autoclipper:   ", autoclipper::cost());
  putln();
  putln("Press Q to quit");
  if (g_wire) putln("Press P to create a paperclip");
  if (g_funds > g_cost_per_spool) putln("Press W to buy a wire spool");
  if (autoclipper::can_buy(g_funds)) putln("Press A to buy an autoclipper");
}

static void load() {
  auto f = savefile::load();
  autoclipper::load(&f);

  f.read(&g_wire_spool);
  f.read(&g_cost_per_box);
  f.read(&g_cost_per_spool);
  f.read(&g_demand);
  f.read(&g_paperclips);
  f.read(&g_wire);
  f.read(&g_funds);
}

static void save() {
  auto f = savefile::save();
  autoclipper::save(&f);

  f.write(&g_wire_spool);
  f.write(&g_cost_per_box);
  f.write(&g_cost_per_spool);
  f.write(&g_demand);
  f.write(&g_paperclips);
  f.write(&g_wire);
  f.write(&g_funds);
}

static void cycle() {
  draw();
  tick();
  save();
  input();
}

int main() try {
  load();
  while (true) cycle(); 
} catch (quit) {
}
