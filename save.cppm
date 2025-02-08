module;
#include <stdio.h>

export module save;
import no;
import sysstd;

export class savefile : no::no {
  FILE * m_file;

  savefile(FILE * f) : m_file { f } {}

public:
  ~savefile() { if (m_file) fclose(m_file); }

  template<typename T>
  void read(T * t) {
    union { char b[64]; T t {}; } u;
    static_assert(sizeof(u) == 64);
    if (m_file) fread(&u, 1, sizeof(u), m_file); 
    *t = u.t;
  }

  template<typename T>
  void write(T * t) {
    union { char b[64]; T t {}; } u;
    static_assert(sizeof(u) == 64);
    u.t = *t;
    fwrite(&u, 1, sizeof(u), m_file); 
  }

  static savefile load() { return { sysstd::fopen("out/save", "rb") }; }
  static savefile save() { return { sysstd::fopen("out/save", "wb") }; }
};
