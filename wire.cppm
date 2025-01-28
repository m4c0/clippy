export module wire;
import dotz;
import numba;
import save;

export namespace wire {
  numba buy(numba funds);
  bool can_buy(numba n);
  numba cost();
  void cut(numba n);
  void load(savefile * f);
  numba stock();
  void save(savefile * f);
  void update_cost();
}

module :private;

static constexpr const auto price_ampliture = 600;

static numba g_wire_spool = 1000;
static numba g_cost_per_spool = 1500;
static float g_cost_ticker = 0;

static numba g_wire = g_wire_spool;

bool wire::can_buy(numba funds) { return funds >= g_cost_per_spool; }
numba wire::stock() { return g_wire; }
void wire::cut(numba n) { g_wire -= n; }

numba wire::cost() {
  // TODO: 1/8 chance of price actually changing every tenth of a second
  // TODO: decrease price by price/1000th every 25s if wire not bought in last 25s
  // TODO: increase price by $0.02 for every buy
  numba increase = price_ampliture * (dotz::cos(g_cost_ticker) * -0.5 + 0.5);
  increase -= increase % 100;
  return g_cost_per_spool + increase;
}

void wire::update_cost() {
  g_cost_ticker += 0.01;
}

numba wire::buy(numba funds) {
  auto c = cost();
  if (funds < c) return 0;
  g_wire += g_wire_spool;
  g_cost_per_spool += 2;
  return c;
}

void wire::load(savefile * f) {
  f->read(&g_wire_spool);
  f->read(&g_cost_per_spool);
  f->read(&g_cost_ticker);
  f->read(&g_wire);
}
void wire::save(savefile * f) {
  f->write(&g_wire_spool);
  f->write(&g_cost_per_spool);
  f->write(&g_cost_ticker);
  f->write(&g_wire);
}
