export module demand;
import dotz;
import save;

export namespace demand {
  unsigned avg_cps();
  unsigned public_demand();
  unsigned price();

  void load(savefile * f);
  void save(savefile * f);
}

module :private;

static struct {
  unsigned cost_per_box = 25;
} g;

static float pd() {
  // (1 + 0.1 * U) * (1.1^M) * Bonuses * (0.8 / P).
  return 100.0 * 0.8 / demand::price();
}

unsigned demand::price() { return g.cost_per_box; }

unsigned demand::public_demand() { return pd() * 10; }

unsigned demand::avg_cps() {
  // min(1, PD/100) * 7 * PD^1.15
  float pd = ::pd();
  return dotz::round(dotz::min(1.0, pd / 100.0) * 7.0 * dotz::pow(pd, 1.15));
}

void demand::load(savefile * f) { f->read(&g);  }
void demand::save(savefile * f) { f->write(&g); }
