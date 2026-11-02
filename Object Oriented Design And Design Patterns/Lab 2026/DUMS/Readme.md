
### Phase 1: Requirement Analysis
*   **Background & Problems:** Manual city management causes delayed complaint resolution, inefficient resource allocation (waste/traffic), and poor communication between citizens and authorities.
*   **Objectives:** 
    *   *Primary:* Develop a centralized platform for automated urban management.
    *   *Specific:* 1) Enable citizen reporting. 2) Automate service dispatch. 3) Integrate IoT data. 4) Provide government analytics.
*   **Actors:** `Citizen` (reports issues), `CityAdministrator` (manages complaints), `TrafficOfficer`/`WasteManager` (handle dispatch), `GovernmentAuthority` (oversees reports).
*   **Boundary:** The DUMS software (UI, controllers) is inside. Physical devices (IoT Sensors, traffic lights) and human actors are outside.
*   **Requirements:**
    *   *Functional:* FR1: Submit complaints. FR2: Dispatch services based on complaint. FR3: Update status to "Resolved". FR4: Generate reports from IoT data.
    *   *Non-Functional:* 1) **Extensibility:** Easy to add new service types (using Factory). 2) **Reliability:** Immediate notifications sent to citizens upon status change.

---

### Phase 2: Object Identification

**1. Entity Classes (Data)**
*   `Citizen`: `id`, `name`, `mobile`
*   `TrafficOfficer`: `signalId`, `signalStatus`
*   `WasteManager`: `binId`, `fillLevel`, `truckId`, `capacity`
*   `EmergencyOfficer`: `requestId`, `emergencyType`, `status`
*   `IoTSensor`: `sensorId`, `sensorType`, `value`

**2. Control Classes (Logic)**
*   `CityAdministrator`: Central Singleton managing workflows (`receiveComplaint`, `createService`, `updateComplaint`).
*   `UrbanServiceFactory` *(Implied)*: Creates specific service types (Traffic, Waste) to decouple logic.
*   `StatusPublisher` *(Implied)*: Manages observer lists and broadcasts status updates to citizens.

**3. Boundary Classes (Interactions)**
*   `GovernmentAuthority`: System entry point (`main()`) triggering analytics.
*   `CitizenApp/Console`: Interface for external citizens to submit data.
*   `SensorGateway` *(Implied)*: Interacts with external physical IoT devices to pull data into the system.

**Actor vs. Representation:** An external actor (e.g., a real person) exists outside the software. Its internal representation (e.g., the `Citizen` class) is a software object that stores their data and methods to process their requests.

---

### Phase 3: CRC Cards

| Class | Responsibilities | Collaborators |
| :--- | :--- | :--- |
| **Citizen** | Register, submit complaints, receive notifications | CityAdministrator |
| **CityAdministrator** | Manage citizens, complaints, services, analytics & reports | Citizen, WasteManager, EmergencyOfficer |
| **TrafficOfficer** | Monitor traffic, control signals, optimize routes | GovernmentAuthority |
| **WasteManager** | Monitor bins, assign trucks, collect waste | CityAdministrator |
| **EmergencyOfficer** | Receive emergency requests, assign officers | CityAdministrator |
| **IoTSensor** | Collect environmental data | GovernmentAuthority |
| **GovernmentAuthority**| Start & coordinate the system | All classes |

