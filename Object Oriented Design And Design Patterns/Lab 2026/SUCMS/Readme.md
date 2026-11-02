
### Phase 1: Requirement Analysis
*   **Background & Problems:** The university currently uses disconnected tools for registration, grading, and notifications. Three problems are: inconsistent registration checks (e.g., credit limits), repeated manual score calculation, and poorly targeted notifications (broadcasting personal results to everyone).
*   **Objectives:**
    *   *Primary:* Develop an object-oriented campus subsystem that validates registrations, computes scores using interchangeable policies, sends appropriate notifications, and accesses an external library through a stable interface.
    *   *Specific:* 1) Create supported account roles via a factory. 2) Enforce registration capacity, duplication, and a 6-credit limit. 3) Calculate/publish scores using a selected policy. 4) Integrate library availability and filter personal results.
*   **Actors & Boundary:**
    *   *Actors:* `Student` (registers, receives results), `Faculty Member` (supplies marks), `Administrator` (creates accounts), `Librarian` (maintains external catalogue).
    *   *Boundary:* The platform contains domain records, controllers, portals, and adapters. External people, 3rd-party systems (Library), and physical devices (RFID) are outside. (Security Officer and IoT System are extension actors not implemented here).
*   **Requirements:**
    *   *Functional:* FR1: Create User accounts and reject unknown roles. FR2: Register student if no duplicates, seat available, and ≤ 6 credits. FR3: Validate marks, calculate via strategy, notify matching student. FR4: Broadcast notices, remove subscribers, translate library counts.
    *   *Non-Functional:* 1) **Maintainability:** Evaluation algorithms and library integrations must be replaceable via interfaces. 2) **Reliability:** Rejected registrations must not change credit totals; invalid marks must not produce notifications.

---

### Phase 2: Object Identification

**1. Entity Classes (Data)**
*   `Student`: `id`, `role` (Domain representation).
*   `Course`: `code`, `credits`, `capacity`, `students`.
*   `Marks`: `exam`, `coursework`, `project` (Assessment value object).
*   `LibraryBook`: `isbn`, `title`, `availableCopies` (Extension).
*   `Notice`: `noticeId`, `message`, `recipientId` (Extension).

**2. Control Classes (Logic)**
*   `CourseRegistrationManager`: Validates duplicates, capacity, and credit limits; updates enrollment.
*   `EvaluationService`: Delegates calculation to the active strategy and publishes the result.
*   `NoticePublisher`: Coordinates subscription, removal, and delivery of notification events.

**3. Boundary Classes (Interactions)**
*   `StudentPortal`: Displays public notices and results matching the student ID.
*   `FacultyPortal`: Displays public campus notices only (filters out personal results).
*   `LibraryAdapter`: Bridges the incompatible external library query format.

**Actor vs. Representation:** An external actor (like the real Librarian) exists outside the system. Internal boundaries (`LibraryAdapter`, `StudentPortal`) act as software proxies to interact with the external library system or the human user's screen.

---

### Phase 3: CRC Cards

| Class | Responsibilities | Collaborators |
| :--- | :--- | :--- |
| **CourseRegistrationManager** | Check duplicate registration, capacity, and credit limit; update enrollment and credit totals. | `Student`, `Course`, `UniversityConfig` |
| **Student** | Retain account identity; report the student role; identify owner of registrations and results. | `UserAccount`, `AccountFactory`, `CourseRegistrationManager`, `EvaluationService` |
| **Course** | Retain code, credits, and capacity; retain enrolled student IDs. | `CourseRegistrationManager`, `Student` |
| **EvaluationService** | Replace evaluation policy; calculate scores; publish score for its student. | `EvaluationStrategy`, `Marks`, `Student`, `NoticePublisher` |
| **NoticePublisher** | Maintain subscribers; remove subscribers; deliver recipient-tagged messages. | `CampusObserver`, `StudentPortal`, `FacultyPortal`, `EvaluationService` |
| **LibraryAdapter** | Query the provider; parse and validate copy counts; return availability or error. | `LibraryService`, `ExternalLibrarySystem` |


