module;
#include <stdio.h>

export module save;
import no;
import sysstd;

export class savefile : no::no {
  FILE * m_file;

  savefile(FILE * f) : m_file { f } {}

public:
  ~savefile() { fclose(m_file); }

  template<typename T>
  void read(T * t) { if (m_file) fread(t, 1, sizeof(T), m_file); }

  template<typename T>
  void write(T * t) { fwrite(t, 1, sizeof(T), m_file); }

  static savefile load() { return { sysstd::fopen("out/save", "rb") }; }
  static savefile save() { return { sysstd::fopen("out/save", "wb") }; }
};
