configure_file(cron_config.in		"${CRON_PROJECT_DIR}/cron_config")

install(FILES		"${CRON_PROJECT_DIR}/cron_config"
	DESTINATION	${CRON_INSTALL_DIR}
	COMPONENT	cron
)
