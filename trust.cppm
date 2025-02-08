export module trust;
import log;
import numba;
import save;

export namespace trust {
  bool enabled();

  unsigned level();
  unsigned processors();
  unsigned memory();

  void update(numba clips);
  void load(savefile * f);
  void save(savefile * f);
};

module :private;

static struct {
  unsigned level {};
  unsigned limit_prev = 1000;
  unsigned limit_curr = 2000;
  unsigned processors {};
  unsigned memory {};
} g;

bool trust::enabled() { return g.level != 0; }

void trust::update(numba clips) {
  if (clips < g.limit_curr) return;

  if (g.level == 0) {
    log("Trust-Constrained Self-Modification enabled");
    g.level = 2;
  } else {
    g.level++;
  }

  auto n = g.limit_prev + g.limit_curr;
  g.limit_prev = g.limit_curr;
  g.limit_curr = n;
}
unsigned trust::level() { return g.level; }
unsigned trust::processors() { return g.processors; }
unsigned trust::memory() { return g.memory; }

void trust::load(savefile * f) { f->read(&g); }
void trust::save(savefile * f) { f->write(&g); }
