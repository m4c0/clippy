export module trust;
import log;
import numba;
import save;

export namespace trust {
  unsigned level();
  bool enabled(numba clips);
  void load(savefile * f);
  void save(savefile * f);
};

module :private;

static struct {
  unsigned level;
} g;

bool trust::enabled(numba clips) {
  if (clips < 2000) return false;
  if (g.level == 0) {
    log("Trust-Constrained Self-Modification enabled");
    g.level = 2;
  }
  return true;
}
unsigned trust::level() {
  return g.level;
}

void trust::load(savefile * f) { f->read(&g); }
void trust::save(savefile * f) { f->write(&g); }
