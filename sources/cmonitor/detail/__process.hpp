/// __process.hpp
#pragma once
#include <sys/types.h>
#include <string>

namespace crf { namespace proc {
namespace detail {
std::string __cmdline_path_of(pid_t pid) noexcept;
std::string __stringify(int __errno);
bool __has_started(pid_t pid) noexcept;

void __handle_forked(pid_t pid, std::string const& cmdline);
} // namespace detail

std::string cmdline_of(pid_t pid);

///throws on error
pid_t start(std::string const& cmdline);

///throws invalid_argument if pid==-1
void kill(pid_t pid);

///please note: as of now no extra cmdline arguments
///are supprted meaning:
///the arguments are ignored / not checked
bool is_alive(pid_t pid, std::string cmdline) noexcept;
} // namespace proc
} // namespace crf

