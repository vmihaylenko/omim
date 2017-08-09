#include "storage/diff_scheme/diff_manager.hpp"
#include "storage/diff_scheme/diff_scheme_checker.hpp"

#include "platform/platform.hpp"

#include "base/assert.hpp"

namespace diff_scheme
{
void Manager::Load(LocalMapsInfo && info)
{
  m_localMapsInfo = std::move(info);
  m_workerThread.Push([this] {
    NameFileInfoMap const diffs = Checker::Check(m_localMapsInfo);
    GetPlatform().RunOnGuiThread([diffs, this] {
      m_diffs = diffs;
      if (diffs.empty())
      {
        m_status = Status::NotAvailable;
        // TODO: Log fall back to the old scheme (Downloader_DiffScheme_OnStart_fallback (Aloha)).
      }
      else
      {
        m_status = Status::Available;
      }

      if (m_deferredUpdateTask.valid())
        m_deferredUpdateTask.get();
    });
  });
}

Status Manager::Status() const
{
  ASSERT_THREAD_CHECKER(m_threadChecker, ());
  return m_status;
}

void Manager::SetStatus(diff_scheme::Status status)
{
  ASSERT_THREAD_CHECKER(m_threadChecker, ());
  m_status = status;
}

FileInfo const & Manager::InfoFor(storage::TCountryId const & countryId) const
{
  ASSERT(HasDiffFor(countryId), ());
  return m_diffs.at(countryId);
}

bool Manager::HasDiffFor(storage::TCountryId const & countryId) const
{
  return m_diffs.find(countryId) != m_diffs.end();
}

bool Manager::IsPossibleToAutoupdate() const
{
  ASSERT_THREAD_CHECKER(m_threadChecker, ());

  if (m_status != Status::Available)
    return false;

  for (auto const & nameVersion : m_localMapsInfo.m_localMaps)
  {
    if (m_diffs.find(nameVersion.first) == m_diffs.end())
      return false;
  }
  return true;
}
}  // namespace diff_scheme
