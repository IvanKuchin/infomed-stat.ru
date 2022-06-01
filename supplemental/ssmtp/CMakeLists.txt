configure_file(ssmtp.conf.in		"${SSMTP_PROJECT_DIR}/ssmtp.conf")

install(FILES		"${SSMTP_PROJECT_DIR}/ssmtp.conf"
	DESTINATION	${SSMTP_INSTALL_DIR}
	COMPONENT	app
)
