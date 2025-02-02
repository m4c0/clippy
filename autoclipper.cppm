export module autoclipper;
import dotz;
import log;
import numba;
import save;

namespace autoclipper {
  export bool can_buy(numba funds);
  export void buy();
  export void check(numba funds);
  export numba cost();
  export numba count();
  export bool enabled();
  export void load(savefile * f);
  export numba run(numba wire);
  export void save(savefile * f);
}

module :private;

static constexpr const numba g_base_cost = 500;

static struct {
  numba count   = 0;
  bool  enabled = false;
} g;

void autoclipper::buy() { g.count++; }

bool autoclipper::can_buy(numba funds) {
  if (!enabled()) return false;
  if (funds < cost()) return false;
  return true;
}

void autoclipper::check(numba funds) {
  if (enabled()) return;
  if (funds < g_base_cost) return;
  g.enabled = true;
  log("Autoclippers enabled");
}

numba autoclipper::cost() {
  return g_base_cost * dotz::pow(1.1, g.count);
}

numba autoclipper::count() { return g.count; }

bool autoclipper::enabled() { return g.enabled; }

void autoclipper::load(savefile * f) { f->read(&g); }

numba autoclipper::run(numba wire) { return dotz::min(wire, g.count); }

void autoclipper::save(savefile * f) { f->write(&g); }
