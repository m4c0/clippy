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

static numba g_demand = 32;

static numba g_paperclips = 0;
static numba g_funds = 0;

static void sell() {
  if (g_paperclips == 0) return;
  if (rng::rand(100) < g_demand) return;
  g_paperclips -= 1;
  g_funds += demand::price();

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

static void buy_spool() {
  auto cost = wire::cost();
  if (g_funds < cost) return;
  wire::buy();
  g_funds -= cost;
}
static void buy_autoclip() {
  auto cost = autoclipper::cost();
  if (g_funds < cost) return;
  autoclipper::buy();
  g_funds -= cost;
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
  auto dem = static_cast<int>(demand::public_demand() * 100);

  putln("\e[1J");
  log_print();
  putln();
  putln("Paperclips:      ", g_paperclips);
  putln("Wire:            ", wire::stock(), " units");
  putln("Funds:           ", g_funds, " dindins");
  putln("Public demand:   ", dem, "%");
  if (autoclipper::enabled()) putln("Autoclippers:    ", autoclipper::count());
  putln();
  putln("Paperclip price: ", demand::price());
  putln();
  putln("Costs of:");
  putln("* Wire spool:    ", wire::cost());
  if (autoclipper::enabled()) putln("* Autoclipper:   ", autoclipper::cost());
  putln();
  putln("Press Q to quit");
  if (wire::stock()) putln("Press P to create a paperclip");
  if (g_funds >= wire::cost()) putln("Press W to buy a wire spool");
  if (autoclipper::can_buy(g_funds)) putln("Press A to buy an autoclipper");
}

static void load() {
  auto f = savefile::load();
  autoclipper::load(&f);
  demand::load(&f);
  wire::load(&f);

  f.read(&g_paperclips);
  f.read(&g_funds);
}

static void save() {
  auto f = savefile::save();
  autoclipper::save(&f);
  demand::save(&f);
  wire::save(&f);

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
