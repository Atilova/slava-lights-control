const delay = ms => new Promise(resolve => setTimeout(resolve, ms))


class AppModule {
    #ui
    #service
    #config

    #running = false
    #updateStatusIntervalId = null
    #preferenceState = {
        triggerOnDrivewayGates: null,
        triggerOnYardGate: null,
        triggerOnFrontDoor: null,
        offDelay: null
    }

    constructor(ui, service, { updateLightStatusInterval = 1000, offDelayUseMinutes = true }) {
        this.#ui = ui
        this.#service = service
        this.#config = {
            updateLightStatusInterval,
            offDelayUseMinutes
        }
    }

    async run() {
        if (this.#running) return


        this.#ui.setOffDelayUnit(
            this.#config.offDelayUseMinutes ? "мин" : "сек"
        )

        this.#running = true
        this.#ui.showLoader()

        const preference = await this.#service.getPreference()
        const status = await this.#service.getLightStatus()

        await delay(200)

        if (preference === null || status === null) {
            this.#ui.showNotification(
                'Не удалось загрузить настройки системы, попробуйте перезагрузить страницу!', {
                    isSuccess: false,
                    timeout: 2500
                }
            )
            return this.#ui.hideLoader()
        }

        const { offDelay, ...rest } = preference
        Object.assign(this.#preferenceState, {
            offDelay: this.#config.offDelayUseMinutes ? offDelay / 60 : offDelay,
            ...rest
        })

        this.#ui.setPreference(this.#preferenceState)
        this.#ui.setLightStatus(status.enabled)
        this.#ui.showNotification('Настройки успешно загружены!')
        this.#ui.hideLoader()

        this.#updateStatusIntervalId = setInterval(
            this.#updateLightStatus.bind(this),
            this.#config.updateLightStatusInterval
        )
    }

    stop() {
        if (!this.#running) return

        this.#running = false

        clearInterval(this.#updateStatusIntervalId)

        ui.setPreference({})
        ui.setLightStatus(false)
    }

    async saveSettings() {
        this.#ui.showLoader('Сохранение...')
        this.#ui.pruneNotification()

        const newPreference = this.#ui.getPreference()
        const { offDelay, ...rest } = newPreference

        const response = await this.#service.savePreference({
            offDelay: this.#config.offDelayUseMinutes ? offDelay * 60 : offDelay,
            ...rest
        })

        await delay(400)

        if (!response?.saved) {
            this.#ui.showNotification(
                'Не удалось сохранить настройки системы, попробуйте ещё раз!', {
                    isSuccess: false
                }
            )
            this.#ui.setPreference(this.#preferenceState)
            return this.#ui.hideLoader()
        }

        Object.assign(this.#preferenceState, newPreference)
        this.#ui.showNotification('Настройки успешно сохранены!')
        this.#ui.hideLoader()
    }

    async #updateLightStatus() {
        const status = await this.#service.getLightStatus()
        if (status === null) {
            this.#ui.showNotification(
                'Не удалось обновить состояние освещения!', {
                    isSuccess: false,
                    timeout: this.#config.updateLightStatusInterval
                }
            )
            return this.#ui.setLightStatus(false)
        }

        this.#ui.setLightStatus(status.enabled)
    }
}
