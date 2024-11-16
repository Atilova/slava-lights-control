const servicePerformGetRequest = async (url, timeout) => {
    try {
        const response = await Promise.race([
            fetch(url),
            new Promise((_, reject) =>
                setTimeout(() => reject(
                    new Error(`Request timed out: ${url}`)
                ), timeout)
            )
        ])

        if (!response.ok) {
            throw new Error(`Error: ${url} ${response.status} ${response.statusText}`)
        }

        return await response.json()
    } catch (error) {
        console.error(error)
        return null
    }
}

const servicePerformPostQueryRequest = async (url, payload, timeout) => {
    const queryParams = new URLSearchParams(payload).toString()
    try {
        const response = await Promise.race([
            fetch(`${url}?${queryParams}`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                }
            }),
            new Promise((_, reject) =>
                setTimeout(() => reject(
                    new Error(`Request timed out: ${url}`)
                ), timeout)
            )
        ])

        if (!response.ok) {
            throw new Error(`Error: ${url} ${response.status} ${response.statusText}`)
        }

        return await response.json()
    } catch (error) {
        console.error(error)
        return null
    }
}


class HttpService {
    #apiUrl
    #timeout

    constructor({ apiUrl, timeout = 5000 }) {
        this.#apiUrl = apiUrl
        this.#timeout = timeout
    }

    async getLightStatus() {
        return servicePerformGetRequest(`${this.#apiUrl}/getLightStatus/`, this.#timeout)
    }

    async getPreference() {
        return servicePerformGetRequest(`${this.#apiUrl}/getPreference/`, this.#timeout)
    }

    async savePreference(preference) {
        return servicePerformPostQueryRequest(`${this.#apiUrl}/savePreference/`, preference, this.#timeout)
    }
}
