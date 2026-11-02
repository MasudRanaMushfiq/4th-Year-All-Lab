import java.util.ArrayList;

class Course {
    String code, name;
    int credit, capacity;
    ArrayList<String> students = new ArrayList<>();

    Course(String code, String name, int credit, int capacity) {
        this.code = code;
        this.name = name;
        this.credit = credit;
        this.capacity = capacity;
    }
}

class RegistrationManager {
    static ArrayList<Course> courses = new ArrayList<>();

    static void viewCourses() {
        for (Course c : courses)
            System.out.println(c.code + " - " + c.name +
                    " | Credit: " + c.credit);
    }

    static void register(Student s, String code) {
        for (Course c : courses) {
            if (c.code.equalsIgnoreCase(code)) {
                int total = 0;
                for (Course x : s.myCourses)
                    total += x.credit;

                if (c.students.contains(s.id))
                    System.out.println("Already registered!");
                else if (c.students.size() >= c.capacity)
                    System.out.println("Course is full!");
                else if (total + c.credit > UniversityConfig.getInstance().maxCredit)
                    System.out.println("Credit limit exceeded!");
                else {
                    c.students.add(s.id);
                    s.myCourses.add(c);
                    System.out.println("Registration successful!");
                }
                return;
            }
        }
        System.out.println("Course not found!");
    }
}
