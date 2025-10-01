#include "LinkCreationAnswer_m.h"
#include "NodeManager.h"
#include "Peer.h"
#include "Vertex.h"

#ifndef __PROT_MES_UT_
#define __PROT_MES_UT_

enum answer_e {
    REFUSED = 0,
    ACCEPTED = 1
};

#if 0
/**
 * sendDirectAcceptanceLinkCreationRequest:
 *
 * \brief Questa funzione effettua l'invio di un messaggio per l'accettazione positiva o negativa per la formazione di un link
 *
 * In particolare, utilizza la sendDirect per inviare un messaggio, in quanto si prevede che non necessariamente sia presente un
 * Channel tra i due nodi (sender e receiver)
 *
 * @param self: Il peer che ha l'intenzione di invare un messaggio
 * @param receiver: Il destinatario del messaggio
 * @param type: Indica se il messaggio é di accettazione o meno
 */
static inline void sendDirectAcceptanceLinkCreationRequest(Vertex* self, int receiver, answer_e type, NodeManager* nm) {
	LinkCreationAnswer *linkCreationAnswer = new LinkCreationAnswer();
	linkCreationAnswer->setSource(self->getId());
	linkCreationAnswer->setDestination(receiver);
	linkCreationAnswer->setAnswer(type);
	self->getPeer()->sendDirect(linkCreationAnswer,nm->getVertexFromId(receiver)->getPeer(),"direct" );
}
#endif

#endif
