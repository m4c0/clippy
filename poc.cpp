#pragma leco tool

import getch;
import print;

static int g_paperclips;

static void draw() {
  putln();
  putln(g_paperclips, " paperclips");
  putln("Press P to create a paperclip");
}

static void process(char input) {
  switch (input) {
    case 'p':
      g_paperclips++;
      break;
    default: break;
  }
}

static void cycle() {
  draw();
  process(getch());
}

int main() {
  while (true) cycle(); 
}
