import java.util.ArrayList;

interface UserAccount {
    String getId();
}

class Student implements UserAccount {
    String id, name;
    ArrayList<Course> myCourses = new ArrayList<>();
    ArrayList<String> notifications = new ArrayList<>();

    Student(String id, String name) {
        this.id = id;
        this.name = name;
    }

    public String getId() {
        return id;
    }
}

class Faculty implements UserAccount {
    String id, name;

    Faculty(String id, String name) {
        this.id = id;
        this.name = name;
    }

    public String getId() {
        return id;
    }
}

class AccountFactory {
    static UserAccount create(String role, String id, String name) {
        if (role.equals("student"))
            return new Student(id, name);
        return new Faculty(id, name);
    }
}
