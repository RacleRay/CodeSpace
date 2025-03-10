use std::fmt::{self, Formatter};
use diesel::result::Error as DieselError;
use serde::Deserialize;
use actix_web::{HttpResponse, ResponseError};
use actix_web::http::StatusCode;
use serde_json::json;


#[derive(Debug, Deserialize)]
pub struct CustomError {
    pub err_status_code: u16,
    pub err_message: String
}


impl CustomError {
    pub fn new(err_status_code: u16, err_message: String) -> CustomError {
        CustomError {
            err_status_code,
            err_message
        }
    }
}


impl fmt::Display for CustomError {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        f.write_str(self.err_message.as_str())
    }
}


impl From<DieselError> for CustomError {
    fn from(error: DieselError) -> CustomError {
        match error {
            DieselError::DatabaseError(_, err) => 
                CustomError::new(409, err.message().to_string()
            ),
            DieselError::NotFound => {
                CustomError::new(404, "employee not found".to_string())
            },
            err => CustomError::new(500,
                format!("Unknown error! {}", err)
            )
        }
    }
}


impl ResponseError for CustomError {
    fn error_response(&self) -> HttpResponse {
        let status_code = match StatusCode::from_u16(self.err_status_code) {
            Ok(status_code) => status_code,
            Err(_) => StatusCode::INTERNAL_SERVER_ERROR
        };

        let error_message = match status_code.as_u16() < 500 {
            true => self.err_message.clone(),
            false => "Internal server error".to_string()
        };

        HttpResponse::build(status_code).json(json!({ "message": error_message }))
    }

}

