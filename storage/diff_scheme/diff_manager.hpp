#pragma once

#include "storage/diff_scheme/diff_types.hpp"
#include "storage/index.hpp"

#include "base/thread_checker.hpp"
#include "base/worker_thread.hpp"

#include <functional>
#include <future>
#include <utility>

namespace diff_scheme
{
class Manager final
{
public:
  FileInfo const & InfoFor(storage::TCountryId const & countryId) const;
  bool IsPossibleToAutoupdate() const;
  bool HasDiffFor(storage::TCountryId const & countryId) const;

  Status Status() const;
  void SetStatus(diff_scheme::Status status);

  void Load(LocalMapsInfo && info);
  void SetDeferredUpdateTask(std::future<void> && task) { m_deferredUpdateTask = std::move(task); }
  void ApplyDiff(std::function<void()> && task) { m_workerThread.Push(std::move(task)); }

private:
  base::WorkerThread m_workerThread;
  diff_scheme::Status m_status = Status::Undefined;
  NameFileInfoMap m_diffs;
  std::future<void> m_deferredUpdateTask;
  LocalMapsInfo m_localMapsInfo;
  ThreadChecker m_threadChecker;
};
}  // namespace diff_scheme

