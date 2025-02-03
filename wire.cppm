export module wire;
import dotz;
import numba;
import save;

export namespace wire {
  void buy();
  numba cost();
  void cut(numba n);
  void load(savefile * f);
  numba stock();
  void save(savefile * f);
  void update_cost();
}

module :private;

static constexpr const auto price_ampliture = 600;

static struct {
  numba wire_spool     = 1000;
  numba cost_per_spool = 1500;
  float cost_ticker    = 0;
  int   market_timer   = 25;

  numba wire = wire_spool;
} g;

numba wire::stock() { return g.wire; }
void wire::cut(numba n) { g.wire -= n; }

numba wire::cost() {
  numba cost = g.cost_per_spool;
  cost = cost + price_ampliture * (dotz::cos(g.cost_ticker) * -0.5 + 0.5);
  cost -= cost % 100;
  return cost;
}

void wire::update_cost() {
  if (g.market_timer == 0) {
    g.cost_per_spool -= g.cost_per_spool / 1000;
    g.market_timer = 25;
  } else {
    g.market_timer--;
  }
  g.cost_ticker += 0.01;
}

void wire::buy() {
  g.wire += g.wire_spool;
  g.cost_per_spool += 2;
  g.market_timer = 25;
}

void wire::load(savefile * f) { f->read(&g);  }
void wire::save(savefile * f) { f->write(&g); }
