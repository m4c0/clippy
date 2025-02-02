export module wire;
import dotz;
import numba;
import save;

export namespace wire {
  void buy();
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
static int g_market_timer = 25;

static numba g_wire = g_wire_spool;

bool wire::can_buy(numba funds) { return funds >= g_cost_per_spool; }
numba wire::stock() { return g_wire; }
void wire::cut(numba n) { g_wire -= n; }

numba wire::cost() {
  numba increase = price_ampliture * (dotz::cos(g_cost_ticker) * -0.5 + 0.5);
  increase -= increase % 100;
  return g_cost_per_spool + increase;
}

void wire::update_cost() {
  if (g_market_timer == 0) {
    g_cost_per_spool -= g_cost_per_spool / 1000;
    g_market_timer = 25;
  } else {
    g_market_timer--;
  }
  g_cost_ticker += 0.01;
}

void wire::buy() {
  g_wire += g_wire_spool;
  g_cost_per_spool += 2;
  g_market_timer = 25;
}

void wire::load(savefile * f) {
  f->read(&g_wire_spool);
  f->read(&g_cost_per_spool);
  f->read(&g_cost_ticker);
  f->read(&g_market_timer);
  f->read(&g_wire);
}
void wire::save(savefile * f) {
  f->write(&g_wire_spool);
  f->write(&g_cost_per_spool);
  f->write(&g_cost_ticker);
  f->write(&g_market_timer);
  f->write(&g_wire);
}
