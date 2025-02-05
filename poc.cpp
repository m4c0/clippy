#pragma leco tool
import autoclipper;
import demand;
import dotz;
import getch;
import log;
import numba;
import print;
import rng;
import save;
import wire;

struct quit {};

static struct {
  numba paperclips = 0;
  numba inventory = 0;
  numba funds = 0;
} g;

static void sell() {
  if (g.inventory == 0) return;
  if (rng::rand(100) > demand::public_demand()) return;

  auto n = dotz::min(g.inventory, demand::box_size());
  g.inventory -= n;
  g.funds += n * demand::price();

  autoclipper::check(g.funds);
}

static void autoclip() {
  auto n = dotz::min(wire::stock(), autoclipper::count());
  wire::cut(n);
  g.paperclips += n;
  g.inventory += n;
}

static void make_paperclip() {
  if (wire::stock() == 0) return;
  wire::cut(1);
  g.paperclips++;
  g.inventory++;
}

static void buy_spool() {
  auto cost = wire::cost();
  if (g.funds < cost) return;
  wire::buy();
  g.funds -= cost;
}
static void buy_autoclip() {
  auto cost = autoclipper::cost();
  if (g.funds < cost) return;
  autoclipper::buy();
  g.funds -= cost;
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
    case '+': demand::price_up(); break;
    case '=': demand::price_up(); break;
    case '-': demand::price_down(); break;
    default:  break;
  }
}

static void draw() {
  putln("\e[1J\e[H");
  log_print();
  putln();
  putln("Paperclips:      ", g.paperclips);
  putln();
  putln("Inventory:       ", g.inventory);
  putln("Wire:            ", wire::stock(), " units");
  putln("Funds:           ", g.funds, " dindins");
  putln("Public demand:   ", demand::public_demand(), "%");
  putln("Avg clips/s:     ", demand::avg_cps());
  if (autoclipper::enabled()) putln("Autoclippers:    ", autoclipper::count());
  putln("Marketing level: ", demand::mkt_level());
  putln();
  putln("Paperclip price: ", demand::price());
  putln();
  putln("Costs of:");
  putln("* Wire spool:    ", wire::cost());
  if (autoclipper::enabled()) putln("* Autoclipper:   ", autoclipper::cost());
  putln("* Mkt upgrade:   ", demand::mkt_cost());
  putln();
  putln("Press Q to quit");
  putln("Press +/- to change price");
  if (wire::stock()) putln("Press P to create a paperclip");
  if (g.funds >= wire::cost()) putln("Press W to buy a wire spool");
  if (autoclipper::can_buy(g.funds)) putln("Press A to buy an autoclipper");
}

static void load() {
  auto f = savefile::load();
  autoclipper::load(&f);
  demand::load(&f);
  wire::load(&f);
  f.read(&g);
}

static void save() {
  auto f = savefile::save();
  autoclipper::save(&f);
  demand::save(&f);
  wire::save(&f);
  f.write(&g);
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
