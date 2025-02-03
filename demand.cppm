export module demand;
import dotz;
import save;

export namespace demand {
  unsigned avg_cps();
  unsigned box_size();
  unsigned mkt_cost();
  unsigned mkt_level();
  unsigned public_demand();
  unsigned price();
  void price_down();
  void price_up();
  void upgrade_mkt();

  void load(savefile * f);
  void save(savefile * f);
}

module :private;

static struct {
  unsigned cost_per_box = 25;
  unsigned mkt_level = 0;
  unsigned mkt_cost = 10000;
} g;

static float pd() {
  // (1 + 0.1 * U) * (1.1^M) * Bonuses * (0.8 / P).
  return 100.0 * dotz::pow(1.1, g.mkt_level) * 0.8 / demand::price();
}

unsigned demand::mkt_cost() { return g.mkt_cost; }
unsigned demand::mkt_level() { return g.mkt_level + 1; }
unsigned demand::price() { return g.cost_per_box; }

unsigned demand::public_demand() { return pd() * 10; }

void demand::price_down() { if (g.cost_per_box > 0) g.cost_per_box--; }
void demand::price_up() { g.cost_per_box++; }

void demand::upgrade_mkt() {
  g.mkt_level++;
  g.mkt_cost *= 2;
}

unsigned demand::avg_cps() {
  // min(1, PD/100) * 7 * PD^1.15
  float pd = ::pd();
  return dotz::round(dotz::min(1.0, pd / 100.0) * 7.0 * dotz::pow(pd, 1.15));
}
unsigned demand::box_size() {
  float pd = ::pd();
  return dotz::floor(0.7 * dotz::pow(pd, 1.15));
}

void demand::load(savefile * f) { f->read(&g);  }
void demand::save(savefile * f) { f->write(&g); }
