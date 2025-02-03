export module demand;
import save;

export namespace demand {
  float public_demand();
  unsigned price();

  void load(savefile * f);
  void save(savefile * f);
}

module :private;

static struct {
  unsigned cost_per_box = 25;
} g;

unsigned demand::price() { return g.cost_per_box; }

float demand::public_demand() {
  // (1 + 0.1 * U) * (1.1^M) * Bonuses * (0.8 / P).
  return 0;
}

// min(1, PD/100) * 7 * PD^1.15

void demand::load(savefile * f) { f->read(&g);  }
void demand::save(savefile * f) { f->write(&g); }
