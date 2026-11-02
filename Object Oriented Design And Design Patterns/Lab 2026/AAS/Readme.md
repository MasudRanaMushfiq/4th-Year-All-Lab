
### Phase 1: Requirement Analysis
*   **Background & Problems:** Manual farm monitoring causes delayed decisions, unnecessary water consumption, and slow responses to weather changes. Three problems are: intermittent soil readings, irrigation running when unnecessary, and delayed weather communication.
*   **Objectives:**
    *   *Primary:* Develop an object-oriented platform that makes traceable irrigation decisions using simulated soil and weather data.
    *   *Specific:* 1) Monitor moisture via a common interface. 2) Select irrigation behavior by active strategy. 3) Use weather data to prevent irrigation if rain is expected. 4) Notify farmers and dashboards.
*   **Actors & Boundary:**
    *   *Actors:* `Farmer` (requests checks, receives alerts), `Farm Administrator` (configures sensors/strategies), `IoT Sensor` (supplies readings), `Weather Service` (supplies forecasts).
    *   *Boundary:* Software includes domain data, controllers, console, and adapters. Physical sensors and external weather providers are *outside*. (Drone and AI systems are extension actors not implemented in the code).
*   **Requirements:**
    *   *Functional:* FR1: Create sensors via factory. FR2: Validate moisture & decide irrigation using strategy + forecast. FR3: Set pump state & publish decision. FR4: Maintain field info via `FarmDataManager` and support observer registration.
    *   *Non-Functional:* 1) **Maintainability:** New sensors/strategies use existing interfaces. 2) **Reliability:** Invalid moisture rejected, pump stays off; equal inputs yield equal decisions.

---

### Phase 2: Object Identification

**1. Entity Classes (Data)**
*   `Field`: `id`, `cropType`
*   `Crop`: `cropID`, `healthStatus`
*   `SoilMoistureSensor`: `sensorID`, `value`
*   `WeatherData`: `rainExpected`, `observationTime`
*   `IrrigationPump`: `pumpID`, `on`

**2. Control Classes (Logic)**
*   `IrrigationController`: Validate readings, coordinate weather/strategy, set pump, publish decisions.
*   `SensorFactory`: Create supported sensors; reject unsupported types.
*   `AlertPublisher`: Coordinate subscriber registration, removal, and delivery.

**3. Boundary Classes (Interactions)**
*   `AASExam`: Console entry point representing farmer requests/admin setup.
*   `WeatherAPIAdapter`: Translate external weather codes into `WeatherData`.
*   `FarmerNotifier`: Deliver decisions to farmer via console output.

**Actor vs. Representation:** External actors (like a real Farmer) exist outside the system. Internal boundaries (`FarmerNotifier`, `WeatherAPIAdapter`) act as software proxies to interact with the human user's screen or the external weather service.

---

### Phase 3: CRC Cards

| Class | Responsibilities | Collaborators |
| :--- | :--- | :--- |
| **IrrigationController** | Validate moisture; obtain weather; apply strategy; coordinate pump & alerts. | `Sensor`, `WeatherService`, `IrrigationStrategy`, `IrrigationPump`, `AlertPublisher` |
| **SoilMoistureSensor** | Retain simulated moisture value; identify sensor type; provide reading. | `SensorFactory`, `IrrigationController` |
| **WeatherAPIAdapter** | Request external code; validate & translate into forecast data. | `WeatherService`, `ExternalWeatherAPI`, `WeatherData` |
| **IrrigationPump** | Maintain activation state; accept state updates; report current state. | `IrrigationController` |
| **AlertPublisher** | Maintain subscribers; remove subscribers; distribute event messages. | `AlertObserver`, `IrrigationController` |
| **FarmerNotifier** | Receive alert messages; display them; retain notification count. | `AlertObserver`, `AlertPublisher` |

