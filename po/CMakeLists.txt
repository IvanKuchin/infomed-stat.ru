message(STATUS "Install locales")

configure_file(ru.pot	"${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.pot" COPYONLY)

# install(CODE "execute_process(COMMAND xgettext -d ${MAIN_DOMAIN_NAME} -o ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.po -s ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.pot)")
install(CODE "execute_process(COMMAND msginit --no-translator --locale ru_RU.utf-8 --output ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.po --input ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.pot)")
install(CODE "execute_process(COMMAND msgfmt ${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.po -o ${LOCALE_INSTALL_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.mo)")

#install(FILES
#	"${LOCALE_PROJECT_DIR}/ru/LC_MESSAGES/${MAIN_DOMAIN_NAME}.po"
#	DESTINATION	"${LOCALE_INSTALL_DIR}/ru/LC_MESSAGES/"
#	PERMISSIONS	OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ 
#)




#add_custom_target("ru.po")
#
#add_custom_command(
#		TARGET "ru.po"
#		DEPENDS ru.pot
#		COMMAND pwd && msginit --no-translator --locale ru_RU --input ru.pot --output ru.po
#		COMMENT "build .po form .pot" VERBATIM
#)
