export module autoclipper;
import dotz;
import log;
import numba;
import save;

namespace autoclipper {
  export bool can_buy(numba funds);
  export numba buy(numba funds);
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

static numba g_count = 0;
static bool g_enabled = false;

numba autoclipper::buy(numba funds) {
  auto c = cost();
  if (!can_buy(funds)) return 0;
  g_count++;
  return c;
}

bool autoclipper::can_buy(numba funds) {
  if (!enabled()) return false;
  if (funds < cost()) return false;
  return true;
}

void autoclipper::check(numba funds) {
  if (enabled()) return;
  if (funds < g_base_cost) return;
  g_enabled = true;
  log("Autoclippers enabled");
}

numba autoclipper::cost() {
  return g_base_cost * dotz::pow(1.1, g_count);
}

numba autoclipper::count() { return g_count; }

bool autoclipper::enabled() { return g_enabled; }

void autoclipper::load(savefile * f) {
  f->read(&g_count);
  f->read(&g_enabled);
}

numba autoclipper::run(numba wire) { return dotz::min(wire, g_count); }

void autoclipper::save(savefile * f) {
  f->write(&g_count);
  f->write(&g_enabled);
}
