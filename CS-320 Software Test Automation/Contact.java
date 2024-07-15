package contact;

public class Contact {
    private String ID;
    private String firstName;
    private String lastName;
    private String number;
    private String address;
    
    // constructor with only ID; fills remaining fields with empty
    public Contact(String ID) {
    	if (ID == null || ID.length() > 10 ) {
    		throw new IllegalArgumentException("Invalid ID");
    	}
    	this.ID = ID;
    	this.firstName = "empty";
    	this.lastName = "empty";
    	this.number = "1111111111";
    	this.address = "empty";
    }
    
    // constructor with each field, throws exceptions if fields are invalid
    public Contact(String ID, String firstName, String lastName, String number, String address) {
		if (ID == null || ID.length() > 10) {
			throw new IllegalArgumentException("Invalid ID");
		}
		if (firstName == null || firstName.length() > 10) {
			throw new IllegalArgumentException("Invalid firstName");
		}
		if (lastName == null || lastName.length() > 10) {
			throw new IllegalArgumentException("Invalid lastName");
		}
		if (number == null || number.length() != 10) {
			throw new IllegalArgumentException("Invalid phone");
		}
		if (address == null || address.length() > 30) {
			throw new IllegalArgumentException("Invalid address");
		}
		
		// after all fields are proven valid, update fields    	
    	this.ID = ID;
    	this.firstName = firstName;
    	this.lastName = lastName;
    	this.number = number;
    	this.address = address;
    }
    
    // getters and setters
	public String getID() {
		return ID;
	}
	public String getFirstName() {
		return firstName;
	}
	public String getLastName() {
		return lastName;
	}
	public String getNumber() {
		return number;
	}
	public String getAddress() {
		return address;
	}
	
	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}
	
	public void setLastName(String lastName) {
		this.lastName = lastName;
	}

	public void setNumber(String number) {
		this.number = number;
	}
	
	public void setAddress(String address) {
		this.address = address;
	}
}
