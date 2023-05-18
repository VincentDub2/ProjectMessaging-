#ifndef DISPLAY_TAG_H
#define DISPLAY_TAG_H


/**
 * @brief Affiche un message de tag avec l'heure.
 *
 * @param message_received Le message à afficher au format "tag:pseudo:sender_pseudo:message".
 * @param pseudo Le pseudo de l'utilisateur actuel.
 * @return N/A.
 */
void display_tag_message(const char *message_received, const char *pseudo);


#endif //DISPLAY_TAG_H
