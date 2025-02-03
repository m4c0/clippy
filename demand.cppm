export module demand;

export namespace demand {
  float public_demand();
}

module :private;

float demand::public_demand() {
  // (1 + 0.1 * U) * (1.1^M) * Bonuses * (0.8 / P).
  return 0;
}

// min(1, PD/100) * 7 * PD^1.15
