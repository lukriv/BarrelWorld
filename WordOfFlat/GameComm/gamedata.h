/*! \brief 
 * 
 * 
 */
 
#ifndef __GAME_DATA_INTERFACE_H__
#define __GAME_DATA_INTERFACE_H__

#include <wx/stream.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"


/*! \brief Game message data class
 * 
 * Implements interface for message data. This interface has only two methods - serialization and deserialization.
 * Important is that all data class (which should be defined within GameComm project) should have its own type
 * correspond with GameMessageType.
 */
class IGameData {
public:
	/*! \brief Get version of data
	 * \return Version of data
	 */
	virtual GameVersionType GetVersion() const = 0;
	
	
	/*! \brief Load data from input stream
	 * \param istream Input stream
	 * \retval FWG_NO_ERROR on no error
	 * \retval Some error code on failture
	 */
	virtual GameErrorCode Load(wxInputStream &istream) = 0;
	
	/*! \brief Store data to output stream
	 * \param ostream Output stream
	 * \retval FWG_NO_ERROR on no error
	 * \retval Some error code on failture
	 */
	virtual GameErrorCode Store(wxOutputStream &ostream) const = 0;
};


#endif //__GAME_DATA_INTERFACE_H__