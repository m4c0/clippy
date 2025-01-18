export module log;
import jute;
import print;

export void log(jute::view);
export void log_print();

module :private;

static constexpr const auto g_term_len = 5;
static jute::view g_term[g_term_len] {};

void log(jute::view msg) {
  for (auto i = 1; i < g_term_len; i++) {
    g_term[i - 1] = g_term[i];
  }
  g_term[g_term_len - 1] = msg;
}
void log_print() {
  for (auto msg: g_term) putln(msg);
}
