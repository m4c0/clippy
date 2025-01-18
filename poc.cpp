#pragma leco tool

import getch;
import print;

static int g_paperclips;

static void cycle() {
  putln("Press P to create a paperclip");
  if (getch() != 'p') return;

  g_paperclips++;

  putln();
  putln(g_paperclips, " paperclips");
  putln();
}

int main() {
  while (true) cycle(); 
}
