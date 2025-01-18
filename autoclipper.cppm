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

static constexpr const numba g_autoclip_min_funds = 500;

static numba g_autoclips = 0;
static numba g_cost_per_autoclip = 0; // TODO: exponent

numba autoclipper::buy(numba funds) {
  if (!can_buy(funds)) return 0;
  g_autoclips++;
  return g_cost_per_autoclip;
}

bool autoclipper::can_buy(numba funds) {
  if (!enabled()) return false;
  if (funds < g_cost_per_autoclip) return false;
  return true;
}

void autoclipper::check(numba funds) {
  if (enabled()) return;
  if (funds < g_autoclip_min_funds) return;
  g_cost_per_autoclip = g_autoclip_min_funds;
  log("Autoclippers enabled");
}

numba autoclipper::cost() { return g_cost_per_autoclip; }

numba autoclipper::count() { return g_autoclips; }

bool autoclipper::enabled() { return g_cost_per_autoclip != 0; }

numba autoclipper::run(numba wire) { return dotz::min(wire, g_autoclips); }
