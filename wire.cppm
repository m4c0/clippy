export module wire;
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
}

module :private;

static numba g_wire_spool = 1000;
static numba g_cost_per_spool = 1500;

static numba g_wire = g_wire_spool;

bool wire::can_buy(numba funds) { return funds >= g_cost_per_spool; }
numba wire::cost() { return g_cost_per_spool; }
numba wire::stock() { return g_wire; }
void wire::cut(numba n) { g_wire -= n; }

numba wire::buy(numba funds) {
  if (funds < g_cost_per_spool) return 0;
  g_wire += g_wire_spool;
  return g_cost_per_spool;
}

void wire::load(savefile * f) {
  f->read(&g_wire_spool);
  f->read(&g_cost_per_spool);
  f->read(&g_wire);
}
void wire::save(savefile * f) {
  f->write(&g_wire_spool);
  f->write(&g_cost_per_spool);
  f->write(&g_wire);
}
