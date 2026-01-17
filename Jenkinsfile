@Library('pipeline-lib') _

pipeline {
    agent any

    parameters {
        booleanParam(
            name: 'All',
            defaultValue: false,
            description: 'Run all even if there are no changes'
        )

		booleanParam(
			name: 'Dev',
			defaultValue: false,
			description: 'Run dev even if there are no changes'
		)

		booleanParam(
			name: 'Prod',
			defaultValue: false,
			description: 'Run prod even if there are no changes'
		)
    }

    environment {
        USERNAME = credentials('vps-username')
        DOMAIN = credentials('vps-domain')
    }

    stages {
        stage('Determine Changes and run pipelines') {
            steps {
                checkout scm

                script {
                    def services = [
                        'bob-robot.api': 'api',
                        'bob-robot.chatter': 'chatter',
                    ]

                    def toTrigger = []
                    services.each { service, path ->
                        if (params.FORCE_RUN || checkMicroservice(path)) {
                            echo "Changes detected in ${service}, will trigger pipelines."
                            toTrigger << service // Add to the list
                        } else {
                            echo "No changes in ${service}, skipping."
                        }
                    }

                    if (toTrigger.isEmpty()) {
                        echo "No projects changed. Nothing to trigger."
                    }
					
					// TODO: ENSURE THIS PIPELINE DOESNT WAIT FOR THESE JOBS TO FINISH

                    toTrigger.each { service -> 
                        echo "Triggering ${service}..."
                        build job: service,
                                parameters: [
                                    booleanParam(name: 'All', value: params.All),
									booleanParam(name: 'Dev', value: params.Dev),
									booleanParam(name: 'Prod', value: params.Prod)
                                ],
                                wait: true // set false for async
                        echo "${service} finished."
                    }
                }
            }
        }
    }
}
