export module autoclipper;
import dotz;
import log;
import numba;

namespace autoclipper {
  export bool can_buy(numba funds);
  export numba buy(numba funds);
  export void check(numba funds);
  export numba cost();
  export numba count();
  export bool enabled();
  export numba run(numba wire);
}

module :private;

static constexpr const numba g_base_cost = 500;

static numba g_count = 0;
static bool g_enabled = false;

numba autoclipper::buy(numba funds) {
  if (!can_buy(funds)) return 0;
  g_count++;
  return cost();
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

numba autoclipper::run(numba wire) { return dotz::min(wire, g_count); }
