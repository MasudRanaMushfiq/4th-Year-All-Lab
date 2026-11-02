import java.util.ArrayList;
import java.util.List;

class Field {
    String name;

    Field(String name) {
        this.name = name;
    }
}

class FarmDataManager {
    List<Field> fields = new ArrayList<>();

    void addField(Field f) {
        fields.add(f);
        System.out.println("Field added successfully!");
    }

    void showFields() {
        if (fields.isEmpty()) {
            System.out.println("No fields available.");
            return;
        }

        for (Field f : fields)
            System.out.println("Field: " + f.name);
    }
}
