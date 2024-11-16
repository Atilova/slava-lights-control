class UI {
    #elements = {
        loader: document.getElementById('loader'),
        loaderText: document.getElementById('loaderText'),
        notificationPopup: document.getElementById('notificationPopup'),
        delayInput: document.getElementById('delayInput'),
        drivewayGateCheckbox: document.getElementById('drivewayGates'),
        yardGateCheckbox: document.getElementById('yardGate'),
        frontDoorCheckbox: document.getElementById('frontDoor'),
        statusSection: document.getElementById('statusSection'),
        offDelayUnit: document.getElementById('offDelayUnit')
    }
    #timeouts = {
        notificationPopupTimeoutId: null,
        offDelayInputValidateTimeoutId: null
    }

    showLoader(text = 'Загрузка...') {
        this.#elements.loaderText.textContent = text
        this.#elements.loader.style.display = 'flex'
    }

    hideLoader() {
        this.#elements.loader.style.display = 'none'
    }

    showNotification(message, { isSuccess = true, timeout = 2000 } = {}) {
        this.pruneNotification()

        this.#elements.notificationPopup.textContent = message
        this.#elements.notificationPopup.style.backgroundColor = isSuccess ? '#333' : '#ff4c4c'
        this.#elements.notificationPopup.classList.add('show')

        this.#timeouts.notificationPopupTimeoutId = setTimeout(() => {
            this.pruneNotification()
        }, timeout)
    }

    pruneNotification() {
        clearTimeout(this.#timeouts.notificationPopupTimeoutId)
        this.#elements.notificationPopup.classList.remove('show')
    }

    adjustOffDelay(shift) {
        const currentValue = parseInt(this.#elements.delayInput.value) || 0

        this.#elements.delayInput.value = Math.min(20, Math.max(1, currentValue + shift))
    }

    setOffDelay() {
        clearTimeout(this.#timeouts.notificationPopupTimeoutId);
        this.#timeouts.notificationPopupTimeoutId = setTimeout(this.validateOffDelay.bind(this), 2000);
    }

    validateOffDelay() {
        clearTimeout(this.#timeouts.notificationPopupTimeoutId);
        const currentValue = parseInt(this.#elements.delayInput.value) || 0

        this.#elements.delayInput.value = Math.min(20, Math.max(1, currentValue))
    }

    getPreference() {
        return {
            triggerOnDrivewayGates: this.#elements.drivewayGateCheckbox.checked,
            triggerOnYardGate: this.#elements.yardGateCheckbox.checked,
            triggerOnFrontDoor: this.#elements.frontDoorCheckbox.checked,
            offDelay: this.#elements.delayInput.value
        }
    }

    setPreference(preference) {
        const {
            triggerOnDrivewayGates,
            triggerOnYardGate,
            triggerOnFrontDoor,
            offDelay
        } = preference

        this.#elements.drivewayGateCheckbox.checked = triggerOnDrivewayGates
        this.#elements.yardGateCheckbox.checked = triggerOnYardGate
        this.#elements.frontDoorCheckbox.checked = triggerOnFrontDoor
        this.#elements.delayInput.value = offDelay
    }

    setLightStatus(isEnabled) {
        if (isEnabled) {
            this.#elements.statusSection.classList.add('active')
        }
        else {
            this.#elements.statusSection.classList.remove('active')
        }
    }

    setOffDelayUnit(unit) {
        this.#elements.offDelayUnit.textContent = unit
    }
}
