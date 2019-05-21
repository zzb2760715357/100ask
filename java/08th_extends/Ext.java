
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
}

class Student extends Persion{

}

public class Ext{
	public static void main(String args[]){
		Student stu = new Student();
		stu.setAge(10);

		System.out.println(stu.getAge());
		
	}
}
