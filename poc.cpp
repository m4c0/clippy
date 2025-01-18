#pragma leco tool
import getch;
import print;
import rng;

typedef int numba;

static numba g_wire_spool = 100;
static numba g_clips_per_box = 10;
static numba g_cost_per_box = 10;
static numba g_cost_per_spool = 10;

static numba g_paperclips = 0;
static numba g_wire = g_wire_spool;
static numba g_funds = 0;

static void draw() {
  putln();
  putln("Paperclips: ", g_paperclips);
  putln("Wire:       ", g_wire, " units");
  putln("Funds:      ", g_funds, " dindins");
  putln();
  putln("Cost of a paperclip box: ", g_cost_per_box);
  putln("Cost of a wire spool:    ", g_cost_per_spool);
  putln();
  putln("Press P to create a paperclip");
  putln("Press W to buy a wire spool");
}

static void sell() {
  if (g_paperclips < g_clips_per_box) return;
  if (rng::randf() > 0.5) return;
  g_paperclips -= g_clips_per_box;
  g_funds += g_cost_per_box;
}

static void make_paperclip() {
  if (g_wire == 0) return putln("Not enought wire");
  g_wire--;
  g_paperclips++;
}

static void buy_spool() {
  if (g_funds < g_cost_per_box) return putln("Not enough dindins");
  g_wire += g_wire_spool;
  g_funds -= g_cost_per_box;
}

static void process(char input) {
  sell();

  switch (input) {
    case 'p': make_paperclip(); break;
    case 'w': buy_spool(); break;
    default:  break;
  }
}

static int input() {
  int res = getch();
  putln("\e[1J");
  return res;
}

static void cycle() {
  draw();
  process(input());
}

int main() {
  while (true) cycle(); 
}
