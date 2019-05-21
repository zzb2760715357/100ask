
class Persion{
	private int age;

	public void setAge(int age){
		if (age < 0 || age > 200)
			age = 0;
		else
			this.age = age;
	}

	public int getAge(){
		return age;
	}

	public void printInfo(){
		System.out.println("age = " + age);
	}

	public Persion(){System.out.println("Persion()");};
	public Persion(int age){
		System.out.println("Persion(int age)");
		this.age = age;
	}	
}

class Student extends Persion{
	private String school;

	public void setSchool(String school){
		this.school = school;
	}

	public String getSchool(){
		return school;
	}

	public Student(String school){
		super(5);
		System.out.println("Student(String school)");
		this.school = school;
	}

	public void printInfo(){
		System.out.println("school = " + school + " age = " + getAge());
	}
}

public class Ext4{
	public static void main(String args[]){
		Student stu = new Student("ustc");
		//stu.setAge(10);

		System.out.println(stu.getAge());
		System.out.println(stu.getSchool());
		stu.printInfo();
		
	}
}
