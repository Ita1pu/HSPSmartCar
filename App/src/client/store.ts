namespace Store {
    export function set(key: string, value: string) {
        try {
            localStorage.setItem(key, value);
        }
        catch(ex) { }
    }

    export function setSession(key: string, value: string) {
        try {
            sessionStorage.setItem(key, value);
        }
        catch(ex) { }
    }

    export function get(key: string): string {
        try {
            return localStorage.getItem(key);
        }
        catch(ex) {
            return null;
        }
    }

    export function getSession(key: string): string {
        try {
            return sessionStorage.getItem(key);
        }
        catch(ex) {
            return null;
        }
    }
}