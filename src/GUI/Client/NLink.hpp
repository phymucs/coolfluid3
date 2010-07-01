#ifndef CF_GUI_Client_NLink_hpp
#define CF_GUI_Client_NLink_hpp

//////////////////////////////////////////////////////////////////////////////

#include <QObject>

#include "GUI/Client/CNode.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace CF {

namespace Common { class CPath; }

namespace GUI {
namespace Client {

  ////////////////////////////////////////////////////////////////////////////

  class NLink :
      public QObject,
      public CNode
  {
    Q_OBJECT

  public:

    typedef boost::shared_ptr<NLink> Ptr;

    /// @brief Constructor
    /// @param name Node name
    NLink(const QString & name, const CF::Common::CPath & targetPath);

    /// @brief Gives the icon associated to this node
    /// @return Returns the icon associated to this node
    /// @note This method should be reimplemented by all subclasses.
    virtual QIcon getIcon() const;

    virtual QString getToolTip() const;

    virtual void getOptions(QList<NodeOption> & params) const;

    CF::Common::CPath getTargetPath() const;

  private slots:

    void goToTarget();

  private :

    CF::Common::CPath m_targetPath;

  }; // class NLink

//////////////////////////////////////////////////////////////////////////////

} // namespace Client
} // namespace GUI
} // namespace CF

//////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_NLink_hpp
