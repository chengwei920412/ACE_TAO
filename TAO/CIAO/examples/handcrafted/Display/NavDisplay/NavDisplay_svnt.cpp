// $Id$

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.cpp       GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/// @@@ Notice that all component and interface names need to be
/// fully qualified as we are creating a new namespace for the CIAO's
/// container glue code.
/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include "NavDisplay_svnt.h"
#include "Cookies.h"

#if !defined (__ACE_INLINE__)
# include "NavDisplay_svnt.inl"
#endif /* __ACE_INLINE__ */

HUDisplay::position_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Context::get_connection_GPSLocation (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return HUDisplay::position::_duplicate (this->ciao_uses_GPSLocation_.in ());
}

// Simplex [receptacle name] connection management operations
void
CIAO_GLUE_HUDisplay::NavDisplay_Context::connect_GPSLocation (HUDisplay::position_ptr c
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::AlreadyConnected,
                   ::Components::InvalidConnection))
{
  if (! CORBA::is_nil (this->ciao_uses_GPSLocation_.in ()))
    ACE_THROW (::Components::AlreadyConnected ());

  if (CORBA::is_nil (c))
    ACE_THROW (::Components::InvalidConnection ());

  // When do we throw InvalidConnection exception?
  this->ciao_uses_GPSLocation_ = HUDisplay::position::_duplicate (c);
}

HUDisplay::position_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Context::disconnect_GPSLocation (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::NoConnection))
{
  if (CORBA::is_nil (this->ciao_uses_GPSLocation_.in ()))
    ACE_THROW (::Components::NoConnection ());

  return this->ciao_uses_GPSLocation_._retn ();
}

// Operations for ::Components::SessionContext interface
CORBA::Object_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Context::get_CCM_object (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ How do I check for IllegalState here?  When it's not in a
  //    callback operation...
  //    ACE_THROW_RETURN (::Components::IllegalState (), 0);

  if (CORBA::is_nil (this->component_.in ()))
    {
      CORBA::Object_var obj =  this->container_->get_objref (this->servant_
                                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->component_ = HUDisplay::NavDisplay::_narrow (obj.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (this->component_.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0); // This should not happen...
    }
  return HUDisplay::NavDisplay::_duplicate (this->component_.in ());
}

//////////////////////////////////////////////////////////////////
// Component Servant Glue code implementation
//////////////////////////////////////////////////////////////////

CIAO_GLUE_HUDisplay::NavDisplay_Servant::NavDisplay_Servant (HUDisplay::CCM_NavDisplay_ptr exe,
                                                             ::Components::CCMHome_ptr h,
                                                             ::CIAO::Session_Container *c)
  : executor_ (HUDisplay::CCM_NavDisplay::_duplicate (exe)),
    container_ (c)
{
  this->context_ = new CIAO_GLUE_HUDisplay::NavDisplay_Context (h, c, this);

  ACE_TRY_NEW_ENV
    {
      Components::SessionComponent_var scom =
        Components::SessionComponent::_narrow (exe
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (! CORBA::is_nil (scom.in ()))
        scom->set_session_context (this->context_
                                   ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
  ACE_ENDTRY;
}

CIAO_GLUE_HUDisplay::NavDisplay_Servant::~NavDisplay_Servant (void)
{
  ACE_TRY_NEW_ENV
    {
      Components::SessionComponent_var scom =
        Components::SessionComponent::_narrow (this->executor_.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (! CORBA::is_nil (scom.in ()))
        scom->ccm_remove (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
  ACE_ENDTRY;
  this->context_->_remove_ref ();
}

// Operations for provides interfaces.

// Operations for consumers interfaces.

// EventConsumer Glue Code implementation
// Inherit from ::Compopnents::EventBConsumerBase
void
CIAO_GLUE_HUDisplay::NavDisplay_Servant::tickConsumer_Refresh_Servant::push_event (::Components::EventBase_ptr ev
                                                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::BadEventType))
{
  HUDisplay::tick_var ev_type = HUDisplay::tick::_downcast (ev);
  if (ev_type != 0)
    {
      this->push_tick (ev_type.in ()
                       ACE_ENV_ARG_PARAMETER);
      return;
    }

  // @@ This include the case when we receive a parent eventtype of [eventtype]

  ACE_THROW (::Components::BadEventType ());
}

// get_component implementation.
HUDisplay::tickConsumer_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_consumer_Refresh (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->consumes_Refresh_.in ()))
    {
      CIAO_GLUE_HUDisplay::NavDisplay_Servant::tickConsumer_Refresh_Servant *svt =
        new CIAO_GLUE_HUDisplay::NavDisplay_Servant::tickConsumer_Refresh_Servant (this->executor_,
                                                                                   this->context_);
      PortableServer::ServantBase_var safe_servant (svt);

      CORBA::Object_var obj = this->container_->install_servant (svt
                                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      HUDisplay::tickConsumer_var eco
        = HUDisplay::tickConsumer::_narrow (obj
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->consumes_Refresh_ = eco;
    }

  return HUDisplay::tickConsumer::_duplicate (this->consumes_Refresh_.in ());
}

// Operations for Navigation interface
CORBA::Object_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::provide_facet (const char * name
                                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException ,
                   Components::InvalidName))
{
  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::FacetDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_all_facets (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::FacetDescriptions_var collection
      = new ::Components::FacetDescriptions (0); // #99 = number of all provided
                                                   // facets including those inherited
                                                   // from parent component(s).
  return collection._retn ();

}

::Components::FacetDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_named_facets (const Components::NameList & names
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

CORBA::Boolean
CIAO_GLUE_HUDisplay::NavDisplay_Servant::same_component (CORBA::Object_ptr object_ref
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (object_ref))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CORBA::Object_var the_other = object_ref->_get_component (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var me = this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return me->_is_equivalent (object_ref
                             ACE_ENV_ARG_PARAMETER);
}

// Operations for Receptacles interface
::Components::Cookie_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::connect (const char * name,
                                                  CORBA::Object_ptr connection
                                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::AlreadyConnected,
                   Components::ExceededConnectionLimit))
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (name, "GPSLocation") == 0)
    {
      HUDisplay::position_var _ciao_conn =
        HUDisplay::position::_narrow (connection
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (_ciao_conn.in ()))
        ACE_THROW_RETURN (::Components::InvalidConnection (), 0);

      this->connect_GPSLocation (_ciao_conn.in ()
                                 ACE_ENV_ARG_PARAMETER);
      return 0;
    }

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

CORBA::Object_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::disconnect (const char * name,
                                                     Components::Cookie_ptr ck
                                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::CookieRequired,
                   Components::NoConnection))
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (name, "GPSLocation") == 0)
    return this->disconnect_GPSLocation (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::ConnectionDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_connections (const char * name
                                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (name, "GPSLocation") == 0)
    {
      ::Components::ConnectionDescriptions_var retv
          = new ::Components::ConnectionDescriptions;
      retv->length (1);

      ::Components::ConnectionDescription_var x
          = new OBV_Components::ConnectionDescription;
      x ->ck (0);
      x ->objref (this->get_connection_GPSLocation (ACE_ENV_SINGLE_ARG_PARAMETER));
      ACE_CHECK_RETURN (0);

      //      retv[0] = x._retn ();

      return retv._retn ();
    }

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::ReceptacleDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_all_receptacles (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::ReceptacleDescriptions_var retv =
      new ::Components::ReceptacleDescriptions (1); // #99 is number of receptacles
                                                      // this component has.
  retv->length (1);
  CORBA::ULong i = 0;

  retv[i] = new OBV_Components::ReceptacleDescription;

  retv[i]->Name ((const char *) "GPSLocation");
  retv[i]->type_id ((const char *) "IDL:HUDisplay/position:1.0");
  retv[i]->is_multiple (0);
  retv[i]->connections (*this->get_connections ("GPSLocation"
                                                ACE_ENV_ARG_PARAMETER));
  return retv._retn ();
}

::Components::ReceptacleDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_named_receptacles (const Components::NameList & names
                                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ::Components::ReceptacleDescriptions_var retv =
      new ::Components::ReceptacleDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      retv[i] = new ::OBV_Components::ReceptacleDescription;
      if (ACE_OS_String::strcmp (names[i].in (), "GPSLocation") == 0)
        {
          retv[i]->Name ((const char *) "GPSLocation");
          retv[i]->type_id ((const char *) "IDL:HUDisplay/position:1.0");
          retv[i]->is_multiple (0);
          retv[i]->connections (*this->get_connections ("GPSLocation"
                                                       ACE_ENV_ARG_PARAMETER));
        }
      else
        ACE_THROW_RETURN (::Components::InvalidName (), 0);
    }
  return retv._retn ();
}

// Operations for Events interface
::Components::EventConsumerBase_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_consumer (const char * sink_name
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ We can omit this if clause if there's no event sinks in this component.
  if (sink_name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (sink_name, "Refresh") == 0)
    return this->get_consumer_Refresh (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::Cookie_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::subscribe (const char * ,
                                                    Components::EventConsumerBase_ptr
                                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::ExceededConnectionLimit))
{
  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::EventConsumerBase_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::unsubscribe (const char * ,
                                                      Components::Cookie_ptr
                                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection))
{
  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

void
CIAO_GLUE_HUDisplay::NavDisplay_Servant::connect_consumer (const char * ,
                                                           Components::EventConsumerBase_ptr
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::AlreadyConnected,
                   Components::InvalidConnection))
{
  ACE_THROW (Components::InvalidName ());
}

::Components::EventConsumerBase_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::disconnect_consumer (const char *
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::NoConnection))
{
  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::ConsumerDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_all_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::ConsumerDescriptions_var retv =
      new ::Components::ConsumerDescriptions (1); // #99 is the number of consumers
                                                    // this component has.
  retv->length (1);

  CORBA::ULong i = 0;
  retv[i] = new OBV_Components::ConsumerDescription;
  retv[i]->Name ("Refresh");
  retv[i]->type_id ("IDL:HUDisplay/tickConsumer:1.0");
  HUDisplay::tickConsumer_var c
    = this->get_consumer_Refresh (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  retv[i]->consumer (c.in ());

  return retv._retn ();
}

::Components::ConsumerDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_named_consumers (const Components::NameList & names
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ::Components::ConsumerDescriptions_var retv =
      new ::Components::ConsumerDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      retv[i] = new OBV_Components::ConsumerDescription;

      if (ACE_OS_String::strcmp (names[i].in (), "Refresh") == 0)
        {
          retv[i]->Name ("Refresh");
          retv[i]->type_id ("IDL:HUDisplay/tickConsumer:1.0");
          HUDisplay::tickConsumer_var c =
            this->get_consumer_Refresh (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          retv[i]->consumer (c.in ());
        }
      else
        ACE_THROW_RETURN (::Components::InvalidName (), 0);
    }
  return retv._retn ();
}

::Components::EmitterDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_all_emitters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::EmitterDescriptions_var retv =
      new ::Components::EmitterDescriptions (0); // #99 is the number of emitters
                                                 // this component has.
  return retv._retn ();
}

::Components::EmitterDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_named_emitters (const Components::NameList & names
                                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ::Components::EmitterDescriptions_var retv =
      new ::Components::EmitterDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      ACE_THROW_RETURN (::Components::InvalidName (), 0);
    }
  return retv._retn ();
}

::Components::PublisherDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_all_publishers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do

  // Need to add interfaces in the Context class to gather the information.
  // Or we can just relay it to the Context object.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::PublisherDescriptions *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_named_publishers (const Components::NameList & names
                                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ to-do

  // Need to add interfaces in the Context class to gather the information.
  // Or we can just relay it to the Context object.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// Operations for CCMObject interface
::CORBA::IRObject_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do: Connect to an IfR?
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::CCMHome_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_ccm_home (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->context_->get_CCM_home (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::Components::PrimaryKeyBase *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_primary_key (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::NoKeyAvailable))
{
  // This is a keyless component.
  ACE_THROW_RETURN (::Components::NoKeyAvailable (), 0);
}

void
CIAO_GLUE_HUDisplay::NavDisplay_Servant::configuration_complete (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidConfiguration))
{
  // @@ to-do
  // No-op.  Don't know how to pass this info to monolithic executor.
}

void
CIAO_GLUE_HUDisplay::NavDisplay_Servant::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // @@ to-do
  // Need to figure out what to do here.  E.g., tear down the all the connections
  // this component has?
}

::Components::ComponentPortDescription *
CIAO_GLUE_HUDisplay::NavDisplay_Servant::get_all_ports (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::ComponentPortDescription_var retv =
    new OBV_Components::ComponentPortDescription;

  ::Components::FacetDescriptions_var facets_desc
      = this->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::ReceptacleDescriptions_var receptacle_desc
      = get_all_receptacles (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::ConsumerDescriptions_var consumer_desc
      = this->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::EmitterDescriptions_var emitter_desc
      = this->get_all_emitters (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::PublisherDescriptions_var publisher_desc
      = this->get_all_publishers (ACE_ENV_SINGLE_ARG_PARAMETER);

  retv->facets (facets_desc.in());
  retv->receptacles (receptacle_desc.in());
  retv->consumers (consumer_desc.in());
  retv->emitters (emitter_desc.in());
  retv->publishers (publisher_desc.in());

  return retv._retn();
}

// get_component implementation.
CORBA::Object_ptr
CIAO_GLUE_HUDisplay::NavDisplay_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->context_);

  if (! CORBA::is_nil(sc.in ()))
    return sc->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  // @@ Do we need to try the following case here?  We are afterall implementing
  // a session component here.
  ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (this->context_);

  if (! CORBA::is_nil(ec.in ()))
    return ec->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
}

void
CIAO_GLUE_HUDisplay::NavDisplay_Servant::_ciao_activate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (this->executor_.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
CIAO_GLUE_HUDisplay::NavDisplay_Servant::_ciao_passivate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (this->executor_.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

//////////////////////////////////////////////////////////////////
// Component Home Glue code implementation
//////////////////////////////////////////////////////////////////

HUDisplay::NavDisplay_ptr
CIAO_GLUE_HUDisplay::NavDisplayHome_Servant::_ciao_activate_component (HUDisplay::CCM_NavDisplay_ptr exe
                                                                       ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var hobj
    = this->container_->get_objref (this
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ::Components::CCMHome_var home
      = ::Components::CCMHome::_narrow (hobj.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CIAO_GLUE_HUDisplay::NavDisplay_Servant *svt =
    new CIAO_GLUE_HUDisplay::NavDisplay_Servant (exe,
                                                 home.in (),
                                                 this->container_);
  PortableServer::ServantBase_var safe (svt);
  PortableServer::ObjectId_var oid;

  CORBA::Object_var objref
    = this->container_->install_component (svt,
                                           oid.out ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  svt->_ciao_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  HUDisplay::NavDisplay_var ho
    = HUDisplay::NavDisplay::_narrow (objref.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (this->component_map_.bind (oid.in (), svt) == 0)
    {
      // @@ what should happen if bind fail?
      safe._retn ();
    }
  return ho._retn ();
}

void
CIAO_GLUE_HUDisplay::NavDisplayHome_Servant::_ciao_passivate_component (HUDisplay::NavDisplay_ptr comp
                                                                        ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid;

  this->container_->uninstall_component (comp,
                                         oid.out ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CIAO_GLUE_HUDisplay::NavDisplay_Servant *servant = 0;
  if (this->component_map_.unbind (oid.in (), servant) == 0)
    {
      PortableServer::ServantBase_var safe (servant);
      servant->_ciao_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  // What happen if unbind failed?

}

// Operations for Implicit Home interface
HUDisplay::NavDisplay_ptr
CIAO_GLUE_HUDisplay::NavDisplayHome_Servant::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  if (this->executor_.in () == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  Components::EnterpriseComponent_var _ciao_ec =
    this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  HUDisplay::CCM_NavDisplay_var _ciao_comp
    = HUDisplay::CCM_NavDisplay::_narrow (_ciao_ec.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->_ciao_activate_component (_ciao_comp.in ()
                                         ACE_ENV_ARG_PARAMETER);
}

// Operations for CCMHome interface
void
CIAO_GLUE_HUDisplay::NavDisplayHome_Servant::remove_component (Components::CCMObject_ptr comp
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  HUDisplay::NavDisplay_var _ciao_comp
    = HUDisplay::NavDisplay::_narrow (comp
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (_ciao_comp.in ()))
    ACE_THROW (CORBA::INTERNAL ()); // What is the right exception to throw here?

  // @@ It seems to me that we need to make sure this is a component
  // generated by this home before calling remove on this component.
  _ciao_comp->remove (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;


  // Removing the object reference?  get the servant from the POA with
  // the objref, and call remove() on the component, deactivate the
  // component, and then remove-ref the servant?
  this->_ciao_passivate_component (_ciao_comp.in ()
                                   ACE_ENV_ARG_PARAMETER);
}
