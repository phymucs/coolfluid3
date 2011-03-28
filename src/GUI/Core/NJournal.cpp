// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/Signal.hpp"

#include "GUI/Core/NJournal.hpp"

/////////////////////////////////////////////////////////////////////////////

using namespace CF::Common;
using namespace CF::Common::XML;

/////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {
namespace Core {

/////////////////////////////////////////////////////////////////////////////

JournalNotifier::JournalNotifier()
{

}

/////////////////////////////////////////////////////////////////////////////

JournalNotifier & JournalNotifier::instance()
{
  static JournalNotifier jn;
  return jn;
}

/////////////////////////////////////////////////////////////////////////////

void JournalNotifier::regist(const NJournal * journal)
{
  cf_assert(journal != nullptr);

  connect(journal, SIGNAL(journalRequest(bool)), this, SIGNAL(journalRequest(bool)));
}

/////////////////////////////////////////////////////////////////////////////

NJournal::NJournal(const QString & name)
  : CNode(name, "CJournal", JOURNAL_NODE)
{
  regist_signal("list_journal", "List journal", "List journal")->
      signal->connect(boost::bind(&NJournal::list_journal, this, _1));

  m_localSignals << "list_journal";

  JournalNotifier::instance().regist(this);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QString NJournal::toolTip() const
{
  return getComponentType();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void NJournal::list_journal(SignalArgs &)
{
  emit journalRequest(false);
}

/////////////////////////////////////////////////////////////////////////////

} // Core
} // GUI
} // CF