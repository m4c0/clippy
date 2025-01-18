#pragma leco tool
import getch;
import print;
import rng;

typedef int numba;

static numba g_wire_spool = 10;

static numba g_paperclips = 0;
static numba g_wire = g_wire_spool;
static numba g_funds = 0;

static void draw() {
  putln();
  putln("Paperclips: ", g_paperclips);
  putln("Wire: ", g_wire, " units");
  putln("Funds: ", g_funds, " dindins");
  putln("Press P to create a paperclip");
  putln("Press W to buy a wire spool");
}

static void sell() {
  if (g_paperclips == 0) return;
  if (rng::randf() > 0.5) return;
  g_paperclips -= 1;
  g_funds += 1;
}

static void make_paperclip() {
  if (g_wire == 0) return;
  g_wire--;
  g_paperclips++;
}

static void buy_spool() {
  if (g_funds == 0) return;
  g_wire += g_wire_spool;
  g_funds--;
}

static void process(char input) {
  sell();

  switch (input) {
    case 'p': make_paperclip(); break;
    case 'w': buy_spool(); break;
    default:  break;
  }
}

static void cycle() {
  draw();
  process(getch());
}

int main() {
  while (true) cycle(); 
}
