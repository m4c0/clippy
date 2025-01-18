#pragma leco tool
import getch;
import print;

typedef int numba;

static numba g_paperclips;
static numba g_wire = 1000;

static void draw() {
  putln();
  putln(g_paperclips, " paperclips");
  putln(g_wire, " wire");
  putln("Press P to create a paperclip");
}

static void make_paperclip() {
  if (g_wire == 0) return;
  g_wire--;
  g_paperclips++;
}

static void process(char input) {
  switch (input) {
    case 'p': make_paperclip(); break;
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
