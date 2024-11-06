
#include "Identifier.h"
#include <cassert>
#include <kiraz/token/Identifier.h>

namespace ast {
Identifier::Identifier(Token::Ptr t) : Node(IDENTIFIER) {
    assert(t->get_id() == IDENTIFIER);
    auto token_id = std::static_pointer_cast<const token::Identifier>(t);
    try {
        m_value = token_id->get_value();
    }
    catch (std::out_of_range &e) {
        // TO DO mark this node as invalid
    }
}


}