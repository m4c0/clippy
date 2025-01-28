#pragma leco tool
import autoclipper;
import dotz;
import getch;
import log;
import numba;
import print;
import rng;
import save;
import wire;

struct quit {};

static numba g_cost_per_box = 25;
static numba g_demand = 32;

static numba g_paperclips = 0;
static numba g_funds = 0;

static void sell() {
  if (g_paperclips == 0) return;
  if (rng::rand(100) < g_demand) return;
  g_paperclips -= 1;
  g_funds += g_cost_per_box;

  autoclipper::check(g_funds);
}

static void autoclip() {
  auto n = autoclipper::run(wire::stock());
  wire::cut(n);
  g_paperclips += n;
}

static void make_paperclip() {
  if (wire::stock() == 0) return;
  wire::cut(1);
  g_paperclips++;
}

// TODO: transactions?
// Both purchase methods rely on this pattern:
// 1. Calculate cost
// 2. Check if funds > cost
// 3. Increase stock
// 4. Deduce funds based on cost at 1
// Mainly due to cost being a function of stock. Next features might have the
// same pattern, therefore, the same chance for overflowing.
static void buy_spool() {
  g_funds -= wire::buy(g_funds);
}
static void buy_autoclip() {
  g_funds -= autoclipper::buy(g_funds);
}

static void tick() {
  sell();
  autoclip();
  wire::update_cost();
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
  putln("Wire:         ", wire::stock(), " units");
  putln("Funds:        ", g_funds, " dindins");
  if (autoclipper::enabled()) putln("Autoclippers: ", autoclipper::count());
  putln();
  putln("Costs of:");
  putln("* Paperclip box: ", g_cost_per_box);
  putln("* Wire spool:    ", wire::cost());
  if (autoclipper::enabled()) putln("* Autoclipper:   ", autoclipper::cost());
  putln();
  putln("Press Q to quit");
  if (wire::stock()) putln("Press P to create a paperclip");
  if (wire::can_buy(g_funds)) putln("Press W to buy a wire spool");
  if (autoclipper::can_buy(g_funds)) putln("Press A to buy an autoclipper");
}

static void load() {
  auto f = savefile::load();
  autoclipper::load(&f);
  wire::load(&f);

  f.read(&g_cost_per_box);
  f.read(&g_demand);
  f.read(&g_paperclips);
  f.read(&g_funds);
}

static void save() {
  auto f = savefile::save();
  autoclipper::save(&f);
  wire::save(&f);

  f.write(&g_cost_per_box);
  f.write(&g_demand);
  f.write(&g_paperclips);
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
